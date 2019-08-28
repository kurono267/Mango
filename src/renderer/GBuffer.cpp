//
// Created by kurono267 on 2019-08-21.
//

#include "GBuffer.hpp"
#include <mango.hpp>

using namespace mango;

struct CameraData {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewProj;
};

struct NodeData {
	glm::mat4 world;
	int material;
};

GBuffer::GBuffer(const spDevice &device, const glm::ivec2& size) : _device(device) {
	std::cout << "GBuffer create" << std::endl;
	_albedo = _device->createTexture(size.x,size.y,1,Format::R16G16B16A16Unorm, TextureType::Input | TextureType::Output);
	_normal = _device->createTexture(size.x,size.y,1,Format::R16G16B16A16Unorm,TextureType::Input | TextureType::Output);
	_pos = _device->createTexture(size.x,size.y,1,Format::R32G32B32A32Sfloat,TextureType::Input | TextureType::Output);
	std::cout << "GBuffer create finish" << std::endl;

	_renderPass = _device->createRenderPass();
	_renderPass->addAttachment(Attachment(_normal->format(),false,0));
	_renderPass->addAttachment(Attachment(_pos->format(),false,1));
	_renderPass->addAttachment(Attachment(_albedo->format(),false,2));
	_renderPass->addAttachment(Attachment(_device->getDepthFormat(),true,3));
	_renderPass->create(_device);

	_framebuffer = _device->createFramebuffer();
	_framebuffer->attachment(_normal->createTextureView());
	_framebuffer->attachment(_pos->createTextureView());
	_framebuffer->attachment(_albedo->createTextureView());
	_framebuffer->depth(size.x,size.y);
	_framebuffer->create(size.x,size.y,_renderPass);

	_cameraUniform.create(_device,sizeof(CameraData));
	_nodeUniform.create(_device,sizeof(NodeData));

	_descSets.resize(2);

	_descSets[0] = _device->createDescSet();
	_descSets[0]->setUniformBuffer(_cameraUniform,0,ShaderStage::Vertex);
	_descSets[0]->setUniformBuffer(_nodeUniform,1,ShaderStage::Vertex);
	_descSets[0]->create();

	_descSets[1] = Material::generalDescSet(_device);

	PipelineInfo pipelineInfo;
	pipelineInfo.setRenderPass(_renderPass);
	pipelineInfo.viewport(Viewport(glm::vec2(0), size));
	pipelineInfo.scissor(glm::ivec2(0), size);
	pipelineInfo.rasterizer(PolygonMode::Fill, CullMode::FrontAndBack);
	pipelineInfo.addShader(ShaderStage::Vertex, "../glsl/renderer/gbuffer.vert");
	pipelineInfo.addShader(ShaderStage::Fragment, "../glsl/renderer/gbuffer.frag");
	pipelineInfo.setDescSet(_descSets);
	pipelineInfo.blend(3);

	_pipeline = _device->createPipeline(pipelineInfo);
}

void GBuffer::update(const spSceneNode &scene) {
	_commandBuffer = _device->createCommandBuffer();
	_commandBuffer->begin();

	_commandBuffer->setClearColor(0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	_commandBuffer->setClearColor(1, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	_commandBuffer->setClearColor(2, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	_commandBuffer->setClearDepthStencil(3, 1.0f, 0.0f);

	_commandBuffer->beginRenderPass(_pipeline->info().getRenderPass(), _framebuffer,
								   RenderArea(_framebuffer->getSize(), glm::ivec2(0)));
	_commandBuffer->bindPipeline(_pipeline);

	uint32_t matId = 0;
	std::vector<spDescSet> descSets(2);
	descSets[0] = _descSets[0];
	scene->run([&](const spSceneNode& node, bool& stop){
		if(!node->getGeometry())return;
		// Get descriptor set of material
		auto geometry = node->getGeometry();
		auto material = geometry->getMaterial();
		auto mesh = geometry->getMesh();
		if(!material || !mesh)return;
		descSets[1] = material->getDescSet();

		NodeData nodeData;
		nodeData.world = node->getWorldTransform();
		nodeData.material = 0;

		_nodeUniform.set(sizeof(NodeData),&nodeData);
		_commandBuffer->bindDescriptorSet(_pipeline,descSets);
		mesh->draw(_commandBuffer);
	});

	_commandBuffer->endRenderPass();

	_commandBuffer->end();
}

void GBuffer::render(const Scene &scene, const spSemaphore& wait, const spSemaphore& finish) {
	// Update Camera
	auto cameraNode = scene.getCameraNode();
	CameraData data;
	data.view = cameraNode->getWorldTransform();
	data.proj = cameraNode->getCamera()->getProj();
	data.viewProj = data.proj*data.view;
	_cameraUniform.set(sizeof(CameraData),&data);

	_device->submit(_commandBuffer,wait,finish);
}

spTexture GBuffer::getAlbedo() const {
	return _albedo;
}

spTexture GBuffer::getNormal() const {
	return _normal;
}

spTexture GBuffer::getPos() const {
	return _pos;
}