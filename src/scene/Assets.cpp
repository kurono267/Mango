//
// Created by kurono267 on 2019-08-02.
//

#include "Assets.hpp"
#include <stb_image.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <iostream>
#include <api/Utils.hpp>
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
				format = Format::R32G32B32Sfloat;
				break;
			case 4:
				format = Format::R32G32B32A32Sfloat;
				break;
			default:
				format = Format::Undefined;
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
				format = Format::R8G8B8A8Srgb;
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

void Assets::freeTextureCache() {
	Assets& instance = Assets::get();
	instance._textureCache.clear();
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
		for (auto tfPrim : tfMesh.primitives) {
			std::vector<sVertex> vertices;
			// Get position data from mesh
			const tinygltf::Accessor &posAccess = tfModel.accessors[tfPrim.attributes["POSITION"]];
			const tinygltf::BufferView &posBufferView = tfModel.bufferViews[posAccess.bufferView];
			vertices.resize(posAccess.count);
			const tinygltf::Buffer &posBuffer = tfModel.buffers[posBufferView.buffer];
			const float *positions = reinterpret_cast<const float *>(&posBuffer.data[posBufferView.byteOffset +
																					 posAccess.byteOffset]);
			for (size_t i = 0; i < posAccess.count; ++i) {
				glm::vec4 pos(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2], 1.0f);
				vertices[i].pos = glm::vec3(pos.x,pos.z,pos.y);
			}

			// Get normal data from mesh
			const tinygltf::Accessor &normalAccess = tfModel.accessors[tfPrim.attributes["NORMAL"]];
			const tinygltf::BufferView &normalBufferView = tfModel.bufferViews[normalAccess.bufferView];
			if (normalAccess.count) {
				const tinygltf::Buffer &normalBuffer = tfModel.buffers[posBufferView.buffer];
				const float *normals = reinterpret_cast<const float *>(&normalBuffer.data[normalBufferView.byteOffset +
																						  normalAccess.byteOffset]);
				for (size_t i = 0; i < normalAccess.count; ++i) {
					glm::vec4 normal(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2], 1.0f);
					vertices[i].normal = glm::normalize(glm::vec3(normal.x,normal.z,normal.y));
				}
			}
			// Get uv data from mesh
			const tinygltf::Accessor &uvAccess = tfModel.accessors[tfPrim.attributes["TEXCOORD_0"]];
			const tinygltf::BufferView &uvBufferView = tfModel.bufferViews[uvAccess.bufferView];
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
			indices.resize(indicesAccess.count);
			const tinygltf::Buffer &indicesBuffer = tfModel.buffers[indicesBufferView.buffer];
			const uint32_t *tfIndices = reinterpret_cast<const uint32_t *>(&indicesBuffer.data[
					indicesBufferView.byteOffset + indicesAccess.byteOffset]);
			for (int i = 0; i < indicesAccess.count; ++i) {
				indices[i] = tfIndices[i];
			}
			spMesh mesh = std::make_shared<Mesh>();
			mesh->create(device,vertices,indices);

			spMaterial mat = std::make_shared<Material>(device);
			// TODO Fix material later

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
				mat->setRoughnessFactor(roughnessFactorItr->second.Factor());
			}
			auto metallicFactorItr = tfMaterial.values.find("metallicFactor");
			if (metallicFactorItr != tfMaterial.values.end()) {
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

			node->setGeometry(Geometry::make(mesh,mat));
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

	auto tfRootNode = tfModel.nodes[0];

	auto path = fs::path(filename).remove_filename();
	spSceneNode node = recursiveLoadNodes(instance._device,instance._materials,path,tfRootNode,tfModel);

	std::cout << "Finish Loading " << std::endl;

	return node;
}
