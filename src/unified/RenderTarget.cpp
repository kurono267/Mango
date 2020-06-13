//
// Created by kurono267 on 2019-11-21.
//

#include "RenderTarget.hpp"
#include "../api/Instance.hpp"

using namespace mango;

RenderTarget::RenderTarget(const glm::ivec2 &size) {
	auto device = Instance::device();
	_framebuffer = device->createFramebuffer(size);
	_renderPass = device->createRenderPass();
	_lastAttachment = 0;
	_isFinished = false;
}

RenderTarget::RenderTarget(const int width, const int height) {
	auto device = Instance::device();
	_framebuffer = device->createFramebuffer(width,height);
	_renderPass = device->createRenderPass();
	_lastAttachment = 0;
	_isFinished = false;
}

RenderTarget::RenderTarget(const spFramebuffer& framebuffer, const spRenderPass& renderPass) : _framebuffer(framebuffer),_renderPass(renderPass),_isFinished(true){}

void RenderTarget::attach(const spTextureView &view) {
	if(_isFinished){
		throw std::logic_error("RenderTarget::attach Try to attach to finished RenderTarget");
	}
	_framebuffer->attachment(view);
	_renderPass->addAttachment(Attachment(view->getTexture()->format(),false,_lastAttachment));
	_lastAttachment++;
}

void RenderTarget::attachDepth() {
	if(_isFinished){
		throw std::logic_error("RenderTarget::attachDepth Try to attachDepth to finished RenderTarget");
	}
	auto device = Instance::device();
	_framebuffer->depth();
	_renderPass->addAttachment(Attachment(device->getDepthFormat(),true,_lastAttachment));
	_lastAttachment++;
}

void RenderTarget::finish() {
	if(_isFinished){
		throw std::logic_error("RenderTarget::finish Try to finish already finished RenderTarget");
	}
	_renderPass->create(false);
	_framebuffer->finish(_renderPass);
	_isFinished = true;
}

spFramebuffer RenderTarget::framebuffer() {
	return _framebuffer;
}

spRenderPass RenderTarget::renderPass() {
	return _renderPass;
}
