//
// Created by kurono267 on 09.07.18.
//

#include <sstream>
#include "Framebuffer.hpp"
#include "Utils.hpp"
#include "Instance.hpp"

using namespace mango;

void Framebuffer::attachment(const spTextureView &imageView) {
	_attachments.push_back(imageView);
}

void Framebuffer::depth() {
	auto device = Instance::device();
	auto depthFormat = device->getDepthFormat();
	_depthBuffer = device->createTexture(_size.x,_size.y,1,depthFormat,TextureUsage::DepthStencilAttachment | TextureUsage::Sampled);
	_depthView = _depthBuffer->createTextureView();
	_attachments.push_back(_depthView);
}

spTextureView Framebuffer::getDepthView(){
	return _depthView;
}

spTexture Framebuffer::getDepthTexture() {
	return _depthBuffer;
}

glm::ivec2 Framebuffer::getSize(){
	return _size;
}

glm::ivec2 Framebuffer::getAttachmentSize(const int id) {
	auto texture = _attachments[id]->getTexture();
	return glm::ivec2(texture->width(),texture->height());
}

Format Framebuffer::getAttachmentFormat(const int id) {
	auto texture = _attachments[id]->getTexture();
	return texture->format();
}

std::string Framebuffer::info(){
	std::stringstream stream;
	stream << "Framebuffer Info" << std::endl;
	stream << "Attachmenets " << _attachments.size() << std::endl;
	for(const auto& a : _attachments){
		auto tex = a->getTexture();
		stream << "width " << tex->width() << " height " << tex->height() << to_string(tex->format()) << std::endl;
	}
	return stream.str();
}
