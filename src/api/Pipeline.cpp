//
// Created by kurono267 on 05.06.18.
//

#include "Pipeline.hpp"
#include "Utils.hpp"

using namespace mango;

PipelineInfo::PipelineInfo(){
	inputAssembly(PrimitiveTopology::TriangleList);
	dynamicScissor();
	dynamicViewport();
	rasterizer();
	multisampling();
	blend();
	depth();
}

void PipelineInfo::inputAssembly(PrimitiveTopology topology){
	_topology = topology;
}

void PipelineInfo::viewport(const float& x,const float& y,const float& width,const float& height,const float& minDepth,const float& maxDepth){
	_viewport = Viewport(glm::vec2(x,y),glm::vec2(width,height),minDepth,maxDepth);
	_dynamicViewport = false;
}

void PipelineInfo::viewport(const Viewport& view){
	_viewport = view;
	_dynamicViewport = false;
}

void PipelineInfo::scissor(const glm::ivec2& offset,const glm::ivec2& size){
	_scissor = glm::ivec4(offset.x,offset.y,size.x,size.y);
	_dynamicScissor = false;
}

void PipelineInfo::dynamicScissor(){
	_dynamicScissor = true;
}
void PipelineInfo::dynamicViewport(){
	_dynamicViewport = true;
}

void PipelineInfo::rasterizer(const PolygonMode& pmode,
				const CullMode& cmode,
				const FrontFace& face,
				const float& lineWidth,
				const bool& depthClamp,
				const bool& discard,
				const bool& depthBias,const glm::vec3& depthBiasFactor){
	_rasterizer.polyMode = pmode;
	_rasterizer.cullMode = cmode;
	_rasterizer.face = face;
	_rasterizer.lineWidth = lineWidth;
	_rasterizer.depthClamp = depthClamp;
	_rasterizer.discard = discard;
	_rasterizer.depthBias = depthBias;
	_rasterizer.depthBiasFactor = depthBiasFactor;
}

void PipelineInfo::multisampling(const SampleCount& count,
				   const bool& sampleShading,
				   const float& minSampleShading,
				   const bool& alphaToCoverageEnable,
				   const bool& alphaToOneEnable){
	_multisampling.samples = count;
	_multisampling.sampleShading = sampleShading;
	_multisampling.alphaToCoverageEnable = alphaToCoverageEnable;
	_multisampling.alphaToOneEnable = alphaToOneEnable;
}

void PipelineInfo::blend(const uint& attachments,const bool& enable,const std::vector<ColorComponent>& writeMask,
			const BlendFactor& srcColorBlendFactor,const BlendFactor& dstColorBlendFactor,
			const BlendOp& colorBlendOp,
			const BlendFactor& srcAlphaBlendFactor,const BlendFactor& dstAlphaBlendFactor,
			const BlendOp& alphaBlendOp){
	_blendAttachments.clear();
	for(int i = 0;i<attachments;++i){
		BlendAttachmentState s;
		s.enable = enable;
		s.writeMask = writeMask;
		s.srcColorBlendFactor = srcColorBlendFactor;
		s.dstColorBlendFactor = dstColorBlendFactor;
		s.colorBlendOp = colorBlendOp;
		s.srcAlphaBlendFactor = srcAlphaBlendFactor;
		s.dstAlphaBlendFactor = dstAlphaBlendFactor;
		s.alphaBlendOp = alphaBlendOp;
		_blendAttachments.push_back(s);
	}
}

void PipelineInfo::depth(const bool& enable, const bool& write,const CompareOp& comp){
	_depthStencil.enable = enable;
	_depthStencil.write = write;
	_depthStencil.comp = comp;
}

const PrimitiveTopology& PipelineInfo::getTopology() const {
	return _topology;
}

const Viewport& PipelineInfo::getViewport() const {
	return _viewport;
}

const glm::ivec4& PipelineInfo::getScissor() const {
	return _scissor;
}

const RasterizationState& PipelineInfo::getRasterizationState() const {
	return _rasterizer;
}

const MultisamplingState& PipelineInfo::getMultisamplingState() const {
	return _multisampling;
}

const std::vector<BlendAttachmentState>& PipelineInfo::getBlendAttachments() const {
	return _blendAttachments;
}

const DepthState& PipelineInfo::getDepthState() const {
	return _depthStencil;
}

bool PipelineInfo::isDynamicScissor() const {
	return _dynamicScissor;
}

bool PipelineInfo::isDynamicViewport() const {
	return _dynamicViewport;
}

const std::unordered_map<ShaderStage, std::string> &PipelineInfo::getShaders() const {
	return _shaders;
}

const spRenderPass &PipelineInfo::getRenderPass() const {
	return _renderPass;
}

const std::vector<spDescSet>& PipelineInfo::getDescSets() const {
	return _descSets;
}

void PipelineInfo::addShader(const ShaderStage &type, const std::string &filename) {
	_shaders[type] = filename;
}

void PipelineInfo::setRenderPass(const spRenderPass &rp) {
	_renderPass = rp;
}

void PipelineInfo::setDescSet(const std::vector<spDescSet> &descSets) {
	_descSets = descSets;
}

void PipelineInfo::setDescSet(const spDescSet &descSet) {
	_descSets.resize(1);
	_descSets[0] = descSet;
}

void RenderPass::addAttachment(const Attachment& a){
	_attachments.push_back(a);
}
