//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_FRAMEBUFFER_HPP
#define MANGO_FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>
#include "Texture.hpp"

namespace mango {

class Framebuffer {
	public:
		Framebuffer() {}
		virtual ~Framebuffer() = default;

		void attachment(const spTextureView& imageView);
		void depth(const int width,const int height); // Create Depth Buffer

		glm::ivec2 getSize(const int attachment = 0);

		spTextureView getDepthView();

		std::string info();

		virtual void create(const int width,const int height,const spRenderPass& renderPass) = 0;
	protected:
		std::vector<spTextureView> _attachments;
		spTexture _depthBuffer;
		spTextureView _depthView;
};

typedef std::shared_ptr<Framebuffer> spFramebuffer;

};

#endif //MANGO_FRAMEBUFFER_HPP
