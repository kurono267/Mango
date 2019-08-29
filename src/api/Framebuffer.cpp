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

void Framebuffer::depth(const int width, const int height) {
	auto device = Instance::device();
	auto depthFormat = device->getDepthFormat();
	_depthBuffer = device->createTexture(width,height,1,depthFormat,TextureType::DepthStencil);
	_depthView = _depthBuffer->createTextureView();
	_attachments.push_back(_depthView);
}

spTextureView Framebuffer::getDepthView(){
	return _depthView;
}

glm::ivec2 Framebuffer::getSize(const int attachment){
	auto texture = _attachments[attachment]->getTexture();
	return glm::ivec2(texture->width(),texture->height());
}

std::string Framebuffer::info(){
	std::stringstream stream;
	stream << "Framebuffer Info" << std::endl;
	stream << "Attachmenets " << _attachments.size() << std::endl;
	for(const auto& a : _attachments){
		stream << "width " << a->getTexture()->width() << " height " << a->getTexture()->height() << to_string(a->getTexture()->format()) << std::endl;
	}
	return stream.str();
}
