//
// Created by kurono267 on 2019-12-05.
//

#include <api/Instance.hpp>
#include <mango.hpp>
#include "ImageBasedLight.hpp"

namespace mango {

struct CubeMatrices {
	glm::mat4 mat[6];
};

ImageBasedLight::ImageBasedLight() : _cubemapSize(1024) {
	init();
}

ImageBasedLight::ImageBasedLight(const spTexture &image, const int cubemapSize) : _image(image),_cubemapSize(cubemapSize) {
	init();
	convert2cubemap();
	filter();
}

void ImageBasedLight::init() {
	glm::mat4 proj = glm::perspective(glm::radians(90.0f),1.0f,0.1f,100.0f);
	CubeMatrices matrices;
	matrices.mat[0] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f));
	matrices.mat[1] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f));
	matrices.mat[2] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	matrices.mat[3] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f));
	matrices.mat[4] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f));
	matrices.mat[5] = proj*glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f), glm::vec3(0.0f, 1.0f,  0.0f));

	_cubeMatrices.create(Instance::device(),sizeof(CubeMatrices),&matrices);

	_cubeMesh = createCube();
}

void ImageBasedLight::convert2cubemap() {
	auto device = Instance::device();

	_cubeTexture = device->createTexture();
	_cubeTexture->createCubeMap(_cubemapSize,_cubemapSize,1,Format::R16G16B16A16Sfloat,TextureType::Input | TextureType::Output);
	_cubeTextureView = _cubeTexture->createTextureViewCubeMap();

	_cubeRenderPass = device->createRenderPass();
	_cubeRenderPass->addAttachment(Attachment(Format::R16G16B16A16Sfloat,false,0));
	_cubeRenderPass->addAttachment(Attachment(device->getDepthFormat(),true,1));
	_cubeRenderPass->create();

	for(int i = 0;i<6;++i){
		_cubeFrameBuffers[i] = device->createFramebuffer(_cubemapSize,_cubemapSize);
		_cubeFrameBuffers[i]->attachment(_cubeTexture->createTextureView(ComponentMapping(),i,1));
		_cubeFrameBuffers[i]->depth();
		_cubeFrameBuffers[i]->finish(_cubeRenderPass);
	}

	_cubeMapDescSet = device->createDescSet();
	_cubeMapDescSet->setUniformBuffer(_cubeMatrices,0,ShaderStage::Vertex);
	_cubeMapDescSet->setTexture(_image->createTextureView(),Sampler(),1,ShaderStage::Fragment);
	_cubeMapDescSet->create();

	PipelineInfo pipelineInfo;
	pipelineInfo.scissor(glm::ivec2(0),glm::ivec2(_cubemapSize,_cubemapSize));
	pipelineInfo.viewport(0,0,_cubemapSize,_cubemapSize);
	pipelineInfo.rasterizer(PolygonMode::Fill,CullMode::Front);
	pipelineInfo.setRenderPass(_cubeRenderPass);
	pipelineInfo.addShader(ShaderStage::Vertex,"../glsl/renderer/image2cube.vert");
	pipelineInfo.addShader(ShaderStage::Fragment,"../glsl/renderer/image2cube.frag");
	pipelineInfo.setDescSet(_cubeMapDescSet);
	pipelineInfo.constant(0,sizeof(int),ShaderStage::Vertex);

	_cubeMapPipeline = device->createPipeline(pipelineInfo);

	_cubeMapCommands = device->createCommandBuffer();

	_cubeMapCommands->begin();

	_cubeMapCommands->setClearColor(0,glm::vec4(0,0,0,1.f));
	_cubeMapCommands->setClearDepthStencil(1);

	for(int face = 0;face<6;++face){
		_cubeMapCommands->beginRenderPass(_cubeRenderPass,_cubeFrameBuffers[face],RenderArea(glm::ivec2(_cubemapSize),glm::ivec2(0)));

		_cubeMapCommands->bindPipeline(_cubeMapPipeline);
		_cubeMapCommands->bindDescriptorSet(_cubeMapPipeline,_cubeMapDescSet);
		_cubeMapCommands->pushConstants(_cubeMapPipeline,0,sizeof(int),ShaderStage::Vertex,&face);

		_cubeMesh->draw(_cubeMapCommands);

		_cubeMapCommands->endRenderPass();
	}

	_cubeMapCommands->end();

	spSemaphore semaphore = device->createSemaphore();
	device->submit(_cubeMapCommands, nullptr,semaphore);
	device->waitIdle();
}

struct FilterConsts {
	int face;
	float rough;
};

void ImageBasedLight::filter() {
	auto device = Instance::device();

	_filterTexture = device->createTexture();
	_filterTexture->createCubeMap(_cubemapSize,_cubemapSize,log2(_cubemapSize),Format::R16G16B16A16Sfloat,TextureType::Input | TextureType::Output);
	_filterTextureView = _filterTexture->createTextureViewCubeMap();

	std::cout << "FilterTexture lod count " << _filterTexture->mipLevels() << std::endl;

	const int roughLevels = _filterTexture->mipLevels();
	_filterFrameBuffers.resize(roughLevels*6);
	for(int m = 0;m<roughLevels;++m){
		int mipsize = _cubemapSize >> m;
		for(int i = 0;i<6;++i){
			_filterFrameBuffers[m*6+i] = device->createFramebuffer(mipsize,mipsize);
			_filterFrameBuffers[m*6+i]->attachment(_filterTexture->createTextureView(ComponentMapping(),i,1,m,1));
			_filterFrameBuffers[m*6+i]->depth();
			_filterFrameBuffers[m*6+i]->finish(_cubeRenderPass);
		}
	}

	_postDescSet = device->createDescSet();
	_postDescSet->setUniformBuffer(_cubeMatrices,0,ShaderStage::Vertex);
	_postDescSet->setTexture(_cubeTextureView,Sampler(),1,ShaderStage::Fragment);
	_postDescSet->create();

	PipelineInfo pipelineInfo;
	pipelineInfo.dynamicScissor();
	pipelineInfo.dynamicViewport();
	pipelineInfo.constant(0,sizeof(FilterConsts),ShaderStage::AllGraphics);
	pipelineInfo.rasterizer(PolygonMode::Fill,CullMode::Front);
	pipelineInfo.setRenderPass(_cubeRenderPass);
	pipelineInfo.addShader(ShaderStage::Vertex,"../glsl/renderer/cubemap_filter.vert");
	pipelineInfo.addShader(ShaderStage::Fragment,"../glsl/renderer/cubemap_filter.frag");
	pipelineInfo.setDescSet(_postDescSet);

	_filterPipeline = device->createPipeline(pipelineInfo);

	_cubeMapPipeline = device->createPipeline(pipelineInfo);

	_filterCommands = device->createCommandBuffer();

	_filterCommands->begin();

	_filterCommands->setClearColor(0,glm::vec4(0,0,0,1.f));
	_filterCommands->setClearDepthStencil(1);

	FilterConsts consts;

	for(int m = 0;m<roughLevels;++m) {
		int mipsize = _cubemapSize >> m;
		for (int face = 0; face < 6; ++face) {
			_filterCommands->beginRenderPass(_cubeRenderPass, _filterFrameBuffers[m*6+face],
											 RenderArea(glm::ivec2(_cubemapSize), glm::ivec2(0)));

			_filterCommands->bindPipeline(_filterPipeline);
			_filterCommands->setViewport(glm::ivec2(mipsize));
			_filterCommands->setScissor(glm::ivec2(mipsize));

			_filterCommands->bindDescriptorSet(_filterPipeline, _postDescSet);

			consts.face = face;
			consts.rough = (float)(m)/((float)roughLevels-1.0f);
			_filterCommands->pushConstants(_filterPipeline, 0, sizeof(FilterConsts), ShaderStage::Vertex, &consts);

			_cubeMesh->draw(_filterCommands);

			_filterCommands->endRenderPass();
		}
	}

	_filterCommands->end();

	spSemaphore semaphore = device->createSemaphore();
	device->submit(_filterCommands, nullptr,semaphore);
	device->waitIdle();
}

void ImageBasedLight::setImage(const spTexture &image) {
	_image = image;
	convert2cubemap();
}

spTexture ImageBasedLight::getImage() {
	return _image;
}

spTexture ImageBasedLight::getCubeMap() {
	return _cubeTexture;
}

spTextureView ImageBasedLight::getCubeMapView() {
	return _cubeTextureView;
}

spTexture ImageBasedLight::getFilter() {
	return _filterTexture;
}

spTextureView ImageBasedLight::getFilterView() {
	return _filterTextureView;
}

}
