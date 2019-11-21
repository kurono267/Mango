//
// Created by kurono267 on 2019-09-04.
//

#include "PBR.hpp"
#include <mango.hpp>

using namespace mango;

PBR::PBR(const spGBuffer &gbuffer, const glm::ivec2& size) : _gBuffer(gbuffer) {
	auto device = Instance::device();
	_lightResult = device->createTexture(size.x,size.y,1,Format::R32G32B32A32Sfloat,TextureType::Input | TextureType::Output);
	auto lightResultView = _lightResult->createTextureView();

	_renderPass = device->createRenderPass();
	_renderPass->addAttachment(Attachment(_lightResult->format(),false,0));
	_renderPass->addAttachment(Attachment(device->getDepthFormat(),true,1));
	_renderPass->create();

	_framebuffer = device->createFramebuffer(size);
	_framebuffer->attachment(lightResultView);
	_framebuffer->depth();
	_framebuffer->finish(_renderPass);

	_uniform.create(device,sizeof(glm::vec3));

	_descSet = device->createDescSet();
	_descSet->setTexture(_gBuffer->getPos()->createTextureView(),Sampler(),0,ShaderStage::Fragment);
	_descSet->setTexture(_gBuffer->getNormal()->createTextureView(),Sampler(),1,ShaderStage::Fragment);
	_descSet->setTexture(_gBuffer->getAlbedo()->createTextureView(),Sampler(),2,ShaderStage::Fragment);
	_descSet->setTexture(_gBuffer->getMaterial()->createTextureView(),Sampler(),3,ShaderStage::Fragment);
	_descSet->setUniformBuffer(_uniform,4,ShaderStage::Fragment);
	_descSet->create();

	PipelineInfo pipelineInfo;
	pipelineInfo.setRenderPass(_renderPass);
	pipelineInfo.viewport(Viewport(glm::vec2(0), size));
	pipelineInfo.scissor(glm::ivec2(0), size);
	pipelineInfo.rasterizer(PolygonMode::Fill, CullMode::FrontAndBack);
	pipelineInfo.addShader(ShaderStage::Vertex, "../glsl/renderer/pbr.vert");
	pipelineInfo.addShader(ShaderStage::Fragment, "../glsl/renderer/pbr.frag");
	pipelineInfo.blend(1,false);
	pipelineInfo.setDescSet(_descSet);

	_pipeline = device->createPipeline(pipelineInfo);

	_quad = createQuad();

	_commandBuffer = device->createCommandBuffer();
	_commandBuffer->begin();

	_commandBuffer->setClearColor(0,glm::vec4(0.0f,1.0f,0.0f,1.0f));
	_commandBuffer->setClearDepthStencil(1,1.0f,0.0f);

	_commandBuffer->beginRenderPass(_renderPass,_framebuffer,RenderArea(_framebuffer->getSize(),glm::ivec2(0)));
	_commandBuffer->bindPipeline(_pipeline);
	_commandBuffer->bindDescriptorSet(_pipeline,_descSet);
	_quad->draw(_commandBuffer);
	_commandBuffer->endRenderPass();

	_commandBuffer->end();
}

void PBR::render(const Scene &scene, const spSemaphore &wait, const spSemaphore &finish) {
	spSceneNode cameraNode = scene.getCameraNode();
	glm::vec3 cameraPos = glm::vec3(cameraNode->getWorldTransform()*glm::vec4(0.f,0.f,0.f,1.f));
	//std::cout << "CameraPos " << glm::to_string(cameraPos) << std::endl;
	_uniform.set(sizeof(glm::vec3),&cameraPos);

	Instance::device()->submit(_commandBuffer,wait,finish);
}

spTexture PBR::getLightResult() {
	return _lightResult;
}
