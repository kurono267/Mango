//
// Created by kurono267 on 2019-09-05.
//

#include "Raytracer.hpp"

using namespace mango;

const int rtGroupSize = 16;

struct RayCameraData {
	glm::mat4 world;
	glm::mat4 invVP;
};

Raytracer::Raytracer(const glm::ivec2 &size) {
	auto device = Instance::device();

	createCameraPipeline(size);

	_albedo = device->createTexture(size.x,size.y,1,Format::R16G16B16A16Unorm, TextureType::Input | TextureType::Storage);
	_normal = device->createTexture(size.x,size.y,1,Format::R16G16B16A16Unorm,TextureType::Input | TextureType::Storage);
	_pos = device->createTexture(size.x,size.y,1,Format::R32G32B32A32Sfloat,TextureType::Input | TextureType::Storage);
	_material = device->createTexture(size.x,size.y,1,Format::R16G16B16A16Sfloat,TextureType::Input | TextureType::Storage);

	_output = device->createDescSet();
	_output->setStorageTexture(_albedo->createTextureView(),Sampler(),0,ShaderStage::Compute);
	_output->setStorageTexture(_normal->createTextureView(),Sampler(),1,ShaderStage::Compute);
	_output->setStorageTexture(_pos->createTextureView(),Sampler(),2,ShaderStage::Compute);
	_output->setStorageTexture(_material->createTextureView(),Sampler(),3,ShaderStage::Compute);
	_output->create();

	_input = device->createDescSet();
	_input->setStorageTexture(_cameraOrigin->createTextureView(),Sampler(),0,ShaderStage::Compute);
	_input->setStorageTexture(_cameraDir->createTextureView(),Sampler(),1,ShaderStage::Compute);
	_input->create();
}

void Raytracer::createCameraPipeline(const glm::ivec2& size) {
	auto device = Instance::device();

	_cameraOrigin = device->createTexture(size.x,size.y,1,Format::R32G32B32A32Sfloat, TextureType::Input | TextureType::Output | TextureType::Storage);
	_cameraDir = device->createTexture(size.x,size.y,1,Format::R32G32B32A32Sfloat,TextureType::Input | TextureType::Output | TextureType::Storage);
	std::cout << "GBuffer create finish" << std::endl;

	_cameraRenderPass = device->createRenderPass();
	_cameraRenderPass->addAttachment(Attachment(_cameraOrigin->format(),false,0));
	_cameraRenderPass->addAttachment(Attachment(_cameraDir->format(),false,1));
	_cameraRenderPass->create();

	_cameraFramebuffer = device->createFramebuffer(size);
	_cameraFramebuffer->attachment(_cameraOrigin->createTextureView());
	_cameraFramebuffer->attachment(_cameraDir->createTextureView());
	_cameraFramebuffer->finish(_cameraRenderPass);

	_cameraUniform.create(device,sizeof(RayCameraData));

	_cameraDescSet = device->createDescSet();
	_cameraDescSet->setUniformBuffer(_cameraUniform,0,ShaderStage::Vertex);
	_cameraDescSet->create();

	PipelineInfo pipelineInfo;
	pipelineInfo.setRenderPass(_cameraRenderPass);
	pipelineInfo.viewport(Viewport(glm::vec2(0), size));
	pipelineInfo.scissor(glm::ivec2(0), size);
	pipelineInfo.rasterizer(PolygonMode::Fill, CullMode::FrontAndBack);
	pipelineInfo.addShader(ShaderStage::Vertex, "../glsl/renderer/raytracer/camera.vert");
	pipelineInfo.addShader(ShaderStage::Fragment, "../glsl/renderer/raytracer/camera.frag");
	pipelineInfo.setDescSet(_cameraDescSet);
	pipelineInfo.blend(2,false);
	pipelineInfo.depth(false, false);

	_cameraPipeline = device->createPipeline(pipelineInfo);

	_quad = createQuad();

	_cameraCommandBuffer = device->createCommandBuffer();
	_cameraCommandBuffer->begin();

	_cameraCommandBuffer->setClearColor(0, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_cameraCommandBuffer->setClearColor(1, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	_cameraCommandBuffer->beginRenderPass(_cameraPipeline->info().getRenderPass(), _cameraFramebuffer,
									RenderArea(_cameraFramebuffer->getSize(), glm::ivec2(0)));
	_cameraCommandBuffer->bindPipeline(_cameraPipeline);

	_cameraCommandBuffer->bindDescriptorSet(_cameraPipeline,_cameraDescSet);

	_quad->draw(_cameraCommandBuffer);

	_cameraCommandBuffer->endRenderPass();

	_cameraCommandBuffer->end();

	_cameraSemaphore = device->createSemaphore();
}

void Raytracer::buildTree(const spSceneNode &sceneNode) {
	auto device = Instance::device();
	sceneNode->run([this,device](const spSceneNode& node, bool& isStop){
		auto geometry = node->getGeometry();
		if(!geometry)return;
		auto mesh = geometry->getMesh();
		if(!mesh)return;
		spBVH bvh = std::make_shared<BVH>(mesh);
		_trees.emplace_back(bvh);
		_nodes.emplace_back(node);

		spDescSet nodeDescSet = device->createDescSet();
		nodeDescSet->setStorageBuffer(mesh->getVertexBuffer(),0,ShaderStage::Compute);
		nodeDescSet->setStorageBuffer(mesh->getIndexBuffer(),1,ShaderStage::Compute);
		nodeDescSet->setStorageBuffer(bvh->getBuffer(),2,ShaderStage::Compute);
		nodeDescSet->create();

		_nodeDescSets.emplace_back(nodeDescSet);
	});
	std::cout << "Mesh count " << _nodes.size() << std::endl;

	std::vector<spDescSet> selectDescSets = {_nodeDescSets[0],_input,_output,_nodes[0]->getGeometry()->getMaterial()->getDescSet()};

	_compute = device->createCompute("../glsl/renderer/raytracer/raytrace.glsl",{_nodeDescSets[0],_input,_output,_nodes[0]->getGeometry()->getMaterial()->getDescSet()});

	_commandBuffer = device->createCommandBuffer();
	_commandBuffer->begin();
	_commandBuffer->bindCompute(_compute);
	_commandBuffer->clearTexture(_pos,glm::vec4(0.f,0.f,0.f,1000.f));
	_commandBuffer->clearTexture(_normal,glm::vec4(0.f));
	_commandBuffer->clearTexture(_albedo,glm::vec4(0.f));
	_commandBuffer->clearTexture(_material,glm::vec4(0.f));
	for(int i = 0;i<_nodeDescSets.size();++i){
		selectDescSets[0] = _nodeDescSets[i];
		selectDescSets[3] = _nodes[i]->getGeometry()->getMaterial()->getDescSet();
		_commandBuffer->bindDescriptorSet(_compute,selectDescSets);
		_commandBuffer->dispatch(_pos->width()/rtGroupSize,_pos->height()/rtGroupSize,1);
	}
	_commandBuffer->end();
}

void Raytracer::render(const Scene& scene, const spSemaphore& wait, const spSemaphore& finish){
	auto device = Instance::device();
	auto cameraNode = scene.getCameraNode();
	RayCameraData data;
	data.world = glm::inverse(cameraNode->getWorldTransform());
	data.invVP = glm::inverse(cameraNode->getCamera()->getProj());
	_cameraUniform.set(sizeof(RayCameraData),&data);

	device->submit(_cameraCommandBuffer,wait,_cameraSemaphore);
	device->submit(_commandBuffer,_cameraSemaphore,finish);
}

spTexture Raytracer::getPos() const {
	return _pos;
}

spTexture Raytracer::getNormal() const {
	return _normal;
}

spTexture Raytracer::getAlbedo() const {
	return _albedo;
}

spTexture Raytracer::getMaterial() const {
	return _material;
}

spTexture Raytracer::getCameraOrigin() const {
	return _cameraOrigin;
}

spTexture Raytracer::getCameraDir() const {
	return _cameraDir;
}
