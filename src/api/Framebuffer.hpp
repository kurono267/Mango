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
		Framebuffer(const glm::ivec2& size) : _size(size) {}
		Framebuffer(const int width, const int height) : _size(width,height) {}
		virtual ~Framebuffer() = default;

		void attachment(const spTextureView& imageView);
		void depth(); // Create Depth Buffer

		glm::ivec2 getSize();
		glm::ivec2 getAttachmentSize(const int id = 0);
		Format getAttachmentFormat(const int id = 0);

		spTextureView getDepthView();
		spTexture getDepthTexture();

		std::string info();

		virtual void finish(const spRenderPass& renderPass) = 0;
	protected:
		glm::ivec2 _size;
		std::vector<spTextureView> _attachments;
		spTexture _depthBuffer;
		spTextureView _depthView;
};

typedef std::shared_ptr<Framebuffer> spFramebuffer;

};

#endif //MANGO_FRAMEBUFFER_HPP
