//
// Created by kurono267 on 2019-08-21.
//

#include "Renderer.hpp"
#include <api/CommandBuffer.hpp>

using namespace mango;

Renderer::Renderer(const spDevice& device,const glm::ivec2 &frameSize)
	: _device(device), _frameSize(frameSize) {
	//_frame = _device->createTexture(_frameSize.x,_frameSize.y,1,Format::R8G8B8A8Srgb,TextureType::Input);
	_gBuffer = std::make_shared<GBuffer>(_device,frameSize);
	_renderPass = _device->getScreenRenderPass();

	const auto& screenbuffers = _device->getScreenbuffers();

	PipelineInfo pipelineInfo;
	pipelineInfo.viewport(Viewport(glm::vec2(0), screenbuffers[0]->getSize()));
	pipelineInfo.scissor(glm::ivec2(0), screenbuffers[0]->getSize());
	pipelineInfo.rasterizer(PolygonMode::Fill, CullMode::FrontAndBack);
	pipelineInfo.addShader(ShaderStage::Vertex, "../glsl/renderer/final.vert");
	pipelineInfo.addShader(ShaderStage::Fragment, "../glsl/renderer/final.frag");

	_frameDescSet = _device->createDescSet();
	_frameDescSet->setTexture(_gBuffer->getAlbedo()->createTextureView(),Sampler(),0,ShaderStage::Fragment);
	_frameDescSet->create();

	pipelineInfo.setDescSet(_frameDescSet);
	pipelineInfo.setRenderPass(_renderPass);

	_framePipeline = _device->createPipeline(pipelineInfo);

	_quadMesh = createQuad(_device);

	_frameCommandBuffers.resize(screenbuffers.size());
	for(int i = 0;i<screenbuffers.size();++i){
		_frameCommandBuffers[i] = _device->createCommandBuffer();
		auto commandBuffer = _frameCommandBuffers[i];
		commandBuffer->begin();

		commandBuffer->setClearColor(0,glm::vec4(0.0f,1.0f,0.0f,1.0f));
		commandBuffer->setClearDepthStencil(1,1.0f,0.0f);

		commandBuffer->beginRenderPass(_renderPass,screenbuffers[i],RenderArea(screenbuffers[i]->getSize(),glm::ivec2(0)));
		commandBuffer->bindPipeline(_framePipeline);
		commandBuffer->bindDescriptorSet(_framePipeline,_frameDescSet);
		_quadMesh->draw(commandBuffer);
		commandBuffer->endRenderPass();

		commandBuffer->end();
	}

	_screenAvailable = device->createSemaphore();
	_renderFinish = device->createSemaphore();
	_gbufferFinish = device->createSemaphore();
}

void Renderer::init(const Scene& scene) {
	_gBuffer->update(scene.rootNode);
}

void Renderer::render(const Scene& scene) {
	auto imageIndex = _device->nextScreen(_screenAvailable);

	_gBuffer->render(scene,_screenAvailable,_gbufferFinish);

	_device->submit(_frameCommandBuffers[imageIndex],_gbufferFinish,_renderFinish);
	_device->present(imageIndex,_renderFinish);
}

Renderer::ptr Renderer::make(const spDevice& device, const glm::ivec2& frameSize) {
	return std::make_shared<Renderer>(device,frameSize);
}
