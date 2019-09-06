//
// Created by kurono267 on 2019-09-05.
//

#include "Raytracer.hpp"

using namespace mango;

struct RayCameraData {
	glm::mat4 world;
	glm::mat4 invVP;
};

Raytracer::Raytracer(const glm::ivec2 &size) {
	auto device = Instance::device();

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

	Uniform vbStorage;
	vbStorage.create(device,64);
	Uniform ibStorage;
	ibStorage.create(device,64);

	_input = device->createDescSet();
	_input->setStorageBuffer(vbStorage,0,ShaderStage::Compute);
	_input->setStorageBuffer(ibStorage,1,ShaderStage::Compute);

	//_compute = device->createCompute("../glsl/renderer/raytracer/trace.glsl",);

	createCameraPipeline(size);
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

	_cameraFramebuffer = device->createFramebuffer();
	_cameraFramebuffer->attachment(_cameraOrigin->createTextureView());
	_cameraFramebuffer->attachment(_cameraDir->createTextureView());
	_cameraFramebuffer->create(size.x,size.y,_cameraRenderPass);

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

	_quad = createQuad(device);

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
	sceneNode->run([this](const spSceneNode& node, bool& isStop){
		auto geometry = node->getGeometry();
		if(!geometry)return;
		auto mesh = geometry->getMesh();
		if(!mesh)return;
		spBVH bvh = std::make_shared<BVH>(mesh);
		_trees.emplace_back(bvh);
		_nodes.emplace_back(node);
	});
	std::cout << "Raytracer::buildTree for scene" << std::endl;
}

void Raytracer::render(const Scene& scene, const spSemaphore& wait, const spSemaphore& finish){
	auto device = Instance::device();
	auto cameraNode = scene.getCameraNode();
	RayCameraData data;
	data.world = cameraNode->getWorldTransform();
	data.invVP = glm::inverse(cameraNode->getCamera()->getProj()*data.world);
	_cameraUniform.set(sizeof(RayCameraData),&data);

	device->submit(_cameraCommandBuffer,wait,finish);
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
