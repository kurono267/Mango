//
// Created by kurono267 on 2019-08-02.
//

#ifndef MANGO_ASSETS_HPP
#define MANGO_ASSETS_HPP

#include <api/Texture.hpp>
#include <unordered_map>
#include "SceneNode.hpp"

namespace mango {

/// Singleton Assets class
/// Using for load and cache textures and meshes
class Assets {
	public:
		/// Init Assets, required call before first using loadTexture or loadModel
		/// @param device Mango device
		static void init(const spDevice& device);
		/// Load texture or return it from cache if it already loaded
		/// @param filename filename of image
		static spTexture loadTexture(const std::string& filename);

		/// Load model or create scene node from data in cache
		static spSceneNode loadModel(const std::string& filename);

		/// Free texture cache
		static void freeTextureCache();
	private:
		Assets() = default;
		~Assets() = default;
		Assets(const Assets&) = delete;
		Assets& operator=(const Assets&) = delete;

		static Assets& get();
	private:
		std::unordered_map<std::string,spTexture> _textureCache;
		std::unordered_map<std::string,spSceneNode> _modelCache;
		spDevice _device;
};

}

#endif //MANGO_ASSETS_HPP
