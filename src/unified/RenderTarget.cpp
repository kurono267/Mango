//
// Created by kurono267 on 2019-11-21.
//

#include "RenderTarget.hpp"
#include "api/Instance.hpp"

using namespace mango;

RenderTarget::RenderTarget(const glm::ivec2 &size) {
	auto device = Instance::device();
	_framebuffer = device->createFramebuffer(size);
	_renderPass = device->createRenderPass();
	_lastAttachment = 0;
}

RenderTarget::RenderTarget(const int width, const int height) {
	auto device = Instance::device();
	_framebuffer = device->createFramebuffer(width,height);
	_renderPass = device->createRenderPass();
	_lastAttachment = 0;
}

void RenderTarget::attach(const spTextureView &view) {
	_framebuffer->attachment(view);
	_renderPass->addAttachment(Attachment(view->getTexture()->format(),false,_lastAttachment));
	_lastAttachment++;
}

void RenderTarget::attachDepth() {
	auto device = Instance::device();
	_framebuffer->depth();
	_renderPass->addAttachment(Attachment(device->getDepthFormat(),true,_lastAttachment));
	_lastAttachment++;
}

void RenderTarget::finish() {
	_renderPass->create(false);
	_framebuffer->finish(_renderPass);
}

spFramebuffer RenderTarget::framebuffer() {
	return _framebuffer;
}

spRenderPass RenderTarget::renderPass() {
	return _renderPass;
}
