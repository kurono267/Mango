//
// Created by kurono267 on 05.06.18.
//

#include "pipeline.hpp"

using namespace mango;

RenderPattern::RenderPattern(){
	inputAssembly(PrimitiveTopology::TriangleList);
	dynamicScissor();
	dynamicViewport();
	rasterizer();
	multisampling();
	blend();
	depth();
}

void RenderPattern::inputAssembly(PrimitiveTopology topology){
	_topology = topology;
}

void RenderPattern::viewport(const float& x,const float& y,const float& width,const float& height,const float& minDepth,const float& maxDepth){
	_viewport = Viewport(glm::vec2(x,y),glm::vec2(width,height),minDepth,maxDepth);
	_dynamicViewport = false;
}

void RenderPattern::viewport(const Viewport& view){
	_viewport = view;
	_dynamicViewport = false;
}

void RenderPattern::scissor(const glm::ivec2& offset,const glm::ivec2& size){
	_scissor = glm::ivec4(offset.x,offset.y,size.x,size.y);
	_dynamicScissor = false;
}

void RenderPattern::dynamicScissor(){
	_dynamicScissor = true;
}
void RenderPattern::dynamicViewport(){
	_dynamicViewport = true;
}

void RenderPattern::rasterizer(const PolygonMode& pmode,
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

void RenderPattern::multisampling(const SampleCount& count,
				   const bool& sampleShading,
				   const float& minSampleShading,
				   const bool& alphaToCoverageEnable,
				   const bool& alphaToOneEnable){
	_multisampling.samples = count;
	_multisampling.sampleShading = sampleShading;
	_multisampling.alphaToCoverageEnable = alphaToCoverageEnable;
	_multisampling.alphaToOneEnable = alphaToOneEnable;
}

void RenderPattern::blend(const uint& attachments,const bool& enable,const std::vector<ColorComponent>& writeMask,
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
		s.dstColorBlendFactor = dstAlphaBlendFactor;
		s.alphaBlendOp = alphaBlendOp;
		_blendAttachments.push_back(s);
	}
}

void RenderPattern::depth(const bool& enable, const bool& write,const CompareOp& comp){
	_depthStencil.enable = enable;
	_depthStencil.write = write;
	_depthStencil.comp = comp;
}

const PrimitiveTopology& RenderPattern::getTopology() const {
	return _topology;
}

const Viewport& RenderPattern::getViewport() const {
	return _viewport;
}

const glm::ivec4& RenderPattern::getScissor() const {
	return _scissor;
}

const RasterizationState& RenderPattern::getRasterizationState() const {
	return _rasterizer;
}

const MultisamplingState& RenderPattern::getMultisamplingState() const {
	return _multisampling;
}

const std::vector<BlendAttachmentState>& RenderPattern::getBlendAttachments() const {
	return _blendAttachments;
}

const DepthState& RenderPattern::getDepthState() const {
	return _depthStencil;
}

bool RenderPattern::isDynamicScissor() const {
	return _dynamicScissor;
}

bool RenderPattern::isDynamicViewport() const {
	return _dynamicViewport;
}

void RenderPass::addAttachment(const Attachment& a){
	_attachments.push_back(a);
}
