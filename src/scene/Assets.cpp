//
// Created by kurono267 on 2019-08-02.
//

#include "Assets.hpp"
#include <stb_image.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <tiny_gltf.h>
#include <iostream>
#include "../api/Utils.hpp"
#include <filesystem>

using namespace mango;
namespace fs = std::filesystem;

spTexture loadTexture(const spDevice& device,const std::string& filename){
	bool isHDR = stbi_is_hdr(filename.c_str());
	bool is16Bit = stbi_is_16_bit(filename.c_str());
	int width; int height; int comp;
	void* data; Format format; int compSize;
	if(isHDR){
		data = (void*)stbi_loadf(filename.c_str(),&width,&height,&comp,0);
		compSize = 4;
		switch(comp){
			case 1:
				format = Format::R32Sfloat;
				break;
			case 2:
				format = Format::R32G32Sfloat;
				break;
			case 3:
			case 4:
				format = Format::R32G32B32A32Sfloat;
				break;
			default:
				format = Format::Undefined;
		}
		if(comp == 3){
			float* pixels = new float[4*width*height];
			for(int y = 0;y<height;++y){
				for(int x = 0;x<width;++x){
					for(int c = 0;c<3;++c){
						pixels[(y*width+x)*4+c] = ((float*)data)[(y*width+x)*3+c];
					}
					pixels[(y*width+x)*4+3] = 1.f;
				}
			}
			data = pixels;
			comp = 4;
		}
	} else if(is16Bit){
		data = (void*)stbi_load_16(filename.c_str(),&width,&height,&comp,0);
		compSize = 2;
		switch(comp){
			case 1:
				format = Format::R16Unorm;
				break;
			case 2:
				format = Format::R16G16Unorm;
				break;
			case 3:
				format = Format::R16G16B16Unorm;
				break;
			case 4:
				format = Format::R16G16B16A16Unorm;
				break;
			default:
				format = Format::Undefined;
		}
	} else {
		data = (void*)stbi_load(filename.c_str(),&width,&height,&comp,0);
		compSize = 1;
		switch(comp){
			case 1:
				format = Format::R8Unorm;
				break;
			case 2:
				format = Format::R8G8Unorm;
				break;
			case 3:
			case 4:
				format = Format::R8G8B8A8Unorm;
				break;
			default:
				format = Format::Undefined;
		}
		if(comp == 3){
			uint8_t* pixels = new uint8_t[4*width*height];
			for(int y = 0;y<height;++y){
				for(int x = 0;x<width;++x){
					for(int c = 0;c<3;++c){
						pixels[(y*width+x)*4+c] = ((uint8_t*)data)[(y*width+x)*3+c];
					}
					pixels[(y*width+x)*4+3] = 255;
				}
			}
			data = pixels;
			comp = 4;
		}
	}

	if(!data){
		std::cout << "Could not load image " << filename << std::endl;
		return nullptr;
	}

	std::cout << "Loaded image " << filename << " width: " << width << " height: " << height << " comp: " << comp << " format: " << to_string(format) << std::endl;

	spTexture texture = device->createTexture(width,height,1,format,TextureType::Input);
	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,width*height*comp*compSize,data);
	texture->set(buffer);

	return texture;
}

spTexture Assets::loadTexture(const std::string &filename) {
	Assets& instance = Assets::get();
	if(!instance._device){
		throw std::runtime_error("Assets::loadTexture Device not setup, call init before using Assets");
	}

	auto texturePair = instance._textureCache.find(filename);
	if(texturePair == instance._textureCache.end()){
		spTexture texture = ::loadTexture(instance._device,filename);
		if(texture)instance._textureCache.emplace(filename,texture);
		return texture;
	} else {
		return texturePair->second;
	}
}

const std::vector<spMaterial>& Assets::getMaterials(){
	Assets& instance = Assets::get();
	return instance._materials;
}

void Assets::freeCache() {
	Assets& instance = Assets::get();
	instance._textureCache.clear();
	instance._materials.clear();
}

Assets &Assets::get() {
	static Assets instance;
	return instance;
}

void Assets::init(const spDevice &device) {
	Assets& instance = Assets::get();
	instance._device = device;
}

spSceneNode recursiveLoadNodes(const spDevice& device,std::vector<spMaterial>& materials,const fs::path& path,const tinygltf::Node& tfNode,tinygltf::Model& tfModel){
	spSceneNode node = std::make_shared<SceneNode>();
	if(tfNode.mesh >= 0) {
		auto tfMesh = tfModel.meshes[tfNode.mesh];
		std::cout << "mesh.primitives count " << tfMesh.primitives.size() << std::endl;
		for (auto tfPrim : tfMesh.primitives) {
			std::vector<sVertex> vertices;
			// Get position data from mesh
			const tinygltf::Accessor &posAccess = tfModel.accessors[tfPrim.attributes["POSITION"]];
			const tinygltf::BufferView &posBufferView = tfModel.bufferViews[posAccess.bufferView];
			vertices.resize(posAccess.count);
			const tinygltf::Buffer &posBuffer = tfModel.buffers[posBufferView.buffer];
			const float *positions = reinterpret_cast<const float *>(&posBuffer.data[posBufferView.byteOffset +
																					 posAccess.byteOffset]);
			int posStride = posAccess.ByteStride(posBufferView);
			std::cout << "posStride " << posStride << std::endl;
			std::cout << "posBufferView.byteOffset " << posBufferView.byteOffset << std::endl;
			std::cout << "posAccess.byteOffset " << posAccess.byteOffset << std::endl;
			for (size_t i = 0; i < posAccess.count; ++i) {
				vertices[i].pos = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
			}

			// Get normal data from mesh
			const tinygltf::Accessor &normalAccess = tfModel.accessors[tfPrim.attributes["NORMAL"]];
			const tinygltf::BufferView &normalBufferView = tfModel.bufferViews[normalAccess.bufferView];
			int normalStride = normalAccess.ByteStride(normalBufferView);
			std::cout << "normalStride " << normalStride << std::endl;
			std::cout << "normalBufferView.byteOffset " << normalBufferView.byteOffset << std::endl;
			std::cout << "normalAccess.byteOffset " << normalAccess.byteOffset << std::endl;
			if (normalAccess.count) {
				const tinygltf::Buffer &normalBuffer = tfModel.buffers[posBufferView.buffer];
				const float *normals = reinterpret_cast<const float *>(&normalBuffer.data[normalBufferView.byteOffset +
																						  normalAccess.byteOffset]);
				for (size_t i = 0; i < normalAccess.count; ++i) {
					vertices[i].normal = glm::normalize(glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
				}
			}
			// Get uv data from mesh
			const tinygltf::Accessor &uvAccess = tfModel.accessors[tfPrim.attributes["TEXCOORD_0"]];
			const tinygltf::BufferView &uvBufferView = tfModel.bufferViews[uvAccess.bufferView];

			int uvStride = uvAccess.ByteStride(uvBufferView);
			std::cout << "uvStride " << uvStride << std::endl;
			if (uvAccess.count) {
				const tinygltf::Buffer &uvBuffer = tfModel.buffers[uvBufferView.buffer];
				const float *uvs = reinterpret_cast<const float *>(&uvBuffer.data[uvBufferView.byteOffset +
																				  uvAccess.byteOffset]);
				for (size_t i = 0; i < uvAccess.count; ++i) {
					vertices[i].uv = glm::vec2(uvs[i * 2 + 0], uvs[i * 2 + 1]);
				}
			}
			// Get indices from mesh
			std::vector<uint32_t> indices;
			const tinygltf::Accessor &indicesAccess = tfModel.accessors[tfPrim.indices];
			const tinygltf::BufferView &indicesBufferView = tfModel.bufferViews[indicesAccess.bufferView];
			int indexSize = tinygltf::GetComponentSizeInBytes(indicesAccess.componentType);

			indices.resize(indicesAccess.count);
			const tinygltf::Buffer &indicesBuffer = tfModel.buffers[indicesBufferView.buffer];
			for (int i = 0; i < indicesAccess.count; ++i) {
				uint32_t index = 0;
				switch (indicesAccess.componentType) {
					case TINYGLTF_COMPONENT_TYPE_BYTE:
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
						index = (uint32_t)*(
								unsigned char *)(indicesBuffer.data.data() +
												indicesBufferView.byteOffset +
												indicesAccess.byteOffset +
												 (i * indexSize));
						break;
					case TINYGLTF_COMPONENT_TYPE_SHORT:
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
						index = (uint32_t)*(
								unsigned short *)(indicesBuffer.data.data() +
												  indicesBufferView.byteOffset +
												  indicesAccess.byteOffset +
												  (i * indexSize));
						break;
					case TINYGLTF_COMPONENT_TYPE_INT:
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
						index = (uint32_t)*(
								unsigned int *)(indicesBuffer.data.data() +
												indicesBufferView.byteOffset +
												indicesAccess.byteOffset +
												(i * indexSize));
						break;
				}
				indices[i] = index;
			}
			spMesh mesh = std::make_shared<Mesh>();
			mesh->create(device,vertices,indices);

			spMaterial mat = Material::create();

			auto tfMaterial = tfModel.materials[tfPrim.material];
			auto baseColorFactorItr = tfMaterial.values.find("baseColorFactor");
			if (baseColorFactorItr != tfMaterial.values.end()) {
				auto colorFactor = baseColorFactorItr->second.ColorFactor();
				mat->setAlbedo(glm::vec4(colorFactor[0], colorFactor[1], colorFactor[2], colorFactor[3]));
			}
			auto baseColorTextureItr = tfMaterial.values.find("baseColorTexture");
			if (baseColorTextureItr != tfMaterial.values.end()) {
				auto textureId = baseColorTextureItr->second.TextureIndex();
				fs::path colorPath = path/tfModel.images[textureId].uri;
				mat->setAlbedo(Assets::loadTexture(colorPath));
			}
			auto roughnessFactorItr = tfMaterial.values.find("roughnessFactor");
			if (roughnessFactorItr != tfMaterial.values.end()) {
				std::cout << "Roughness Factor " << roughnessFactorItr->second.Factor() << std::endl;
				mat->setRoughnessFactor(roughnessFactorItr->second.Factor());
			}
			auto metallicFactorItr = tfMaterial.values.find("metallicFactor");
			if (metallicFactorItr != tfMaterial.values.end()) {
				std::cout << "Metallic Factor " << metallicFactorItr->second.Factor() << std::endl;
				mat->setMetallicFactor(metallicFactorItr->second.Factor());
			}
			auto metallicRoughnessTextureItr = tfMaterial.values.find("metallicRoughnessTexture");
			if(metallicRoughnessTextureItr != tfMaterial.values.end()){
				auto textureId = metallicRoughnessTextureItr->second.TextureIndex();
				fs::path mrPath = path/tfModel.images[textureId].uri;
				mat->setMetallicRoughness(Assets::loadTexture(mrPath));
			}

			materials.push_back(mat);
			mat->setID(materials.size()-1);

			node->setGeometry(Geometry::create(mesh,mat));
		}
	}
	if(tfNode.matrix.size() == 16){
		glm::mat4 transform(tfNode.matrix[0],tfNode.matrix[1],tfNode.matrix[2],tfNode.matrix[3],
							tfNode.matrix[4],tfNode.matrix[5],tfNode.matrix[6],tfNode.matrix[7],
							tfNode.matrix[8],tfNode.matrix[9],tfNode.matrix[10],tfNode.matrix[11],
							tfNode.matrix[12],tfNode.matrix[13],tfNode.matrix[14],tfNode.matrix[15]);
		node->setTransform(transform);
	} else {
		if(tfNode.scale.size() == 3){
			node->setScale(glm::vec3(tfNode.scale[0],tfNode.scale[1],tfNode.scale[2]));
		}
		if(tfNode.rotation.size() == 4){
			node->setRotation(glm::quat(tfNode.rotation[0],tfNode.rotation[1],tfNode.rotation[2],tfNode.rotation[3]));
		}
		if(tfNode.translation.size() == 3){
			node->setPos(glm::vec3(tfNode.translation[0],tfNode.translation[1],tfNode.translation[2]));
		}
	}

	for(int i = 0;i<tfNode.children.size();++i){
		spSceneNode children = recursiveLoadNodes(device,materials,path,tfModel.nodes[tfNode.children[i]],tfModel);
		node->addChild(children);
	}
	return node;
}

spSceneNode Assets::loadModel(const std::string &filename) {
	auto& instance = Assets::get();

	tinygltf::Model tfModel;
	tinygltf::TinyGLTF tfLoader;
	std::string err;
	std::string warn;

	bool ret = tfLoader.LoadASCIIFromFile(&tfModel, &err, &warn, filename);
	if(!ret){
		std::cout << "loadModel " << filename << " Failed " << err << std::endl;
		return nullptr;
	}
	std::cout << "loadModel " << filename << std::endl;
	if(!warn.empty())std::cout << "Warnings! " << warn << std::endl;

	auto path = fs::path(filename).remove_filename();
	spSceneNode rootNode = std::make_shared<SceneNode>();
	for(int i = 0;i<tfModel.nodes.size();++i) {
		auto tfRootNode = tfModel.nodes[i];

		spSceneNode node = recursiveLoadNodes(instance._device, instance._materials, path, tfRootNode, tfModel);
		rootNode->addChild(node);
	}

	std::cout << "Finish Loading " << std::endl;

	return rootNode;
}
