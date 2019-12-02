//
// Created by kurono267 on 2019-11-21.
//

#ifndef MANGO_RENDERTARGET_HPP
#define MANGO_RENDERTARGET_HPP

#include <api/Framebuffer.hpp>
#include <api/Pipeline.hpp>

namespace mango {

class RenderTarget {
	public:
		typedef std::shared_ptr<RenderTarget> ptr;
		static ptr make(const glm::ivec2& size) {return std::make_shared<RenderTarget>(size);}
		static ptr make(const int width, const int height) {return std::make_shared<RenderTarget>(width,height);}
	public:
		RenderTarget(const glm::ivec2& size);
		RenderTarget(const int width, const int height);
		RenderTarget(const spFramebuffer& framebuffer, const spRenderPass& renderPass);

		void attach(const spTextureView& view);
		void attachDepth();

		void finish();

		spFramebuffer framebuffer();
		spRenderPass renderPass();
	protected:
		spFramebuffer _framebuffer;
		spRenderPass _renderPass;
		int _lastAttachment;
		bool _isFinished;
};

typedef std::shared_ptr<RenderTarget> spRenderTarget;

}

#endif //MANGO_RENDERTARGET_HPP
