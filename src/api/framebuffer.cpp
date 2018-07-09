//
// Created by kurono267 on 09.07.18.
//

#include "framebuffer.hpp"

using namespace mango;

void Framebuffer::attachment(const spTextureView &imageView) {
	_attachments.push_back(imageView);
}

void Framebuffer::depth(const int width, const int height) {
	auto depthFormat = _device->getDepthFormat();
	_depthBuffer = _device->createTexture(width,height,1,depthFormat,TextureType::Output);
	_depthView = _depthBuffer->createTextureView();
	_attachments.push_back(_depthView);
}
