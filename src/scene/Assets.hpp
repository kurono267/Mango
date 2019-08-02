//
// Created by kurono267 on 2019-08-02.
//

#ifndef MANGO_ASSETS_HPP
#define MANGO_ASSETS_HPP

#include <api/Texture.hpp>
#include <unordered_map>

namespace mango {

/// Singleton Assets class
/// Using for load and cache textures and meshes
class Assets {
	public:
		static void init(const spDevice& device);

		static spTexture loadTexture(const std::string& filename);

		static void freeTextureCache();
	private:
		Assets() = default;
		~Assets() = default;
		Assets(const Assets&) = delete;
		Assets& operator=(const Assets&) = delete;

		static Assets& get();
	private:
		std::unordered_map<std::string,spTexture> _textureCache;
		spDevice _device;
};

}

#endif //MANGO_ASSETS_HPP
