//
// Created by kurono267 on 2019-08-02.
//

#include "Assets.hpp"
#include <stbi/stb_image.h>
#include <iostream>
#include <api/Utils.hpp>

using namespace mango;

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
				format = Format::R8Srgb;
				break;
			case 2:
				format = Format::R8G8Srgb;
				break;
			case 3:
				format = Format::R8G8B8Srgb;
				break;
			case 4:
				format = Format::R8G8B8A8Srgb;
				break;
			default:
				format = Format::Undefined;
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
