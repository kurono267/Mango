//
// Created by kurono267 on 2019-08-21.
//

#include "Renderer.hpp"
#include <api/CommandBuffer.hpp>

using namespace mango;

Renderer::Renderer(const spDevice& device,const glm::ivec2 &frameSize)
	: _device(device), _frameSize(frameSize) {
	//_frame = _device->createTexture(_frameSize.x,_frameSize.y,1,Format::R8G8B8A8Srgb,TextureType::Input);
	std::cout << "FRAMESIZE:" << _frameSize.x << "," << _frameSize.y << std::endl;
	_gBuffer = std::make_shared<GBuffer>(_device,frameSize);
	_pbr = std::make_shared<PBR>(_gBuffer,frameSize);
	_raytracer = std::make_shared<Raytracer>(frameSize);
	auto screenSize = _device->getScreenSize();
	_renderTargets = _device->getScreenRenderTargets();

	PipelineInfo pipelineInfo;
	pipelineInfo.viewport(Viewport(glm::vec2(0), screenSize));
	pipelineInfo.scissor(glm::ivec2(0), screenSize);
	pipelineInfo.rasterizer(PolygonMode::Fill, CullMode::FrontAndBack);
	pipelineInfo.addShader(ShaderStage::Vertex, "../glsl/renderer/final.vert");
	pipelineInfo.addShader(ShaderStage::Fragment, "../glsl/renderer/final.frag");

	_frameDescSet = _device->createDescSet();
	//_frameDescSet->setTexture(_raytracer->getAlbedo()->createTextureView(),Sampler(),0,ShaderStage::Fragment);
	_frameDescSet->setTexture(_pbr->getLightResult()->createTextureView(),Sampler(),0,ShaderStage::Fragment);
	_frameDescSet->create();

	pipelineInfo.setDescSet(_frameDescSet);
	pipelineInfo.setRenderPass(_renderTargets[0]->renderPass());

	_framePipeline = _device->createPipeline(pipelineInfo);

	_quadMesh = createQuad();

	_frameCommandBuffers.resize(_renderTargets.size());
	for(int i = 0;i<_renderTargets.size();++i){
		_frameCommandBuffers[i] = _device->createCommandBuffer();
		auto commandBuffer = _frameCommandBuffers[i];
		commandBuffer->begin();

		commandBuffer->setClearColor(0,glm::vec4(0.0f,1.0f,0.0f,1.0f));
		commandBuffer->setClearDepthStencil(1,1.0f,0.0f);

		commandBuffer->beginRenderPass(_renderTargets[i]->renderPass(),_renderTargets[i]->framebuffer(),RenderArea(screenSize,glm::ivec2(0)));
		commandBuffer->bindPipeline(_framePipeline);
		commandBuffer->bindDescriptorSet(_framePipeline,_frameDescSet);
		_quadMesh->draw(commandBuffer);
		commandBuffer->endRenderPass();

		commandBuffer->end();
	}

	_screenAvailable = device->createSemaphore();
	_renderFinish = device->createSemaphore();
	_gbufferFinish = device->createSemaphore();
	_pbrFinish = device->createSemaphore();
	_raytracerFinish = device->createSemaphore();
}

void Renderer::init(const Scene& scene) {
	_gBuffer->update(scene.rootNode);
	_raytracer->buildTree(scene.rootNode);
}

void Renderer::render(const Scene& scene) {
	auto imageIndex = _device->nextScreen(_screenAvailable);

	_gBuffer->render(scene,_screenAvailable,_gbufferFinish);
	_pbr->render(scene,_gbufferFinish,_pbrFinish);
	_raytracer->render(scene,_pbrFinish,_raytracerFinish);

	_device->submit(_frameCommandBuffers[imageIndex],_raytracerFinish,_renderFinish);
	_device->present(imageIndex,_renderFinish);
}

Renderer::ptr Renderer::make(const spDevice& device, const glm::ivec2& frameSize) {
	return std::make_shared<Renderer>(device,frameSize);
}
