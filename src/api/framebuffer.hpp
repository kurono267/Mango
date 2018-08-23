//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_FRAMEBUFFER_HPP
#define MANGO_FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>
#include "texture.hpp"

namespace mango {

class Framebuffer {
	public:
		Framebuffer(const spDevice& device) : _device(device) {}
		virtual ~Framebuffer() = default;

		void attachment(const spTextureView& imageView);
		void depth(const int width,const int height); // Create Depth Buffer

		std::string info();

		virtual void create(const int width,const int height,const spPipeline& pipeline) = 0;
	protected:
		spDevice _device;

		std::vector<spTextureView> _attachments;
		spTexture _depthBuffer;
		spTextureView _depthView;
};

typedef std::shared_ptr<Framebuffer> spFramebuffer;

};

#endif //MANGO_FRAMEBUFFER_HPP
