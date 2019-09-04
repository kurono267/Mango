//
// Created by kurono267 on 12.03.18.
//

#pragma once

#include "default.hpp"
#include "Types.hpp"
#include "Device.hpp"
#include "DescSet.hpp"

#include <vector>
#include <unordered_map>

namespace mango {

typedef unsigned int uint;

struct MultisamplingState {
	SampleCount samples;
	bool sampleShading;
	float minSampleShading;
	bool alphaToCoverageEnable;
	bool alphaToOneEnable;
};

struct BlendAttachmentState {
	bool enable;
	std::vector<ColorComponent> writeMask;
	BlendFactor srcColorBlendFactor;
	BlendFactor dstColorBlendFactor;
	BlendOp colorBlendOp;
	BlendFactor srcAlphaBlendFactor;
	BlendFactor dstAlphaBlendFactor;
	BlendOp alphaBlendOp;
};

struct DepthState {
	bool enable;
	bool write;
	CompareOp comp;
};

struct Attachment {
	Attachment(const Format& f, const bool& d, const int i,const SampleCount s = SampleCount::s1){
		format = f;
		depth = d;
		index = i;
		samples = s;
	}
	Format format;
	SampleCount samples;
	bool depth;
	int index;
};

class RenderPass {
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;

		void addAttachment(const Attachment& a);

		virtual void create(const bool isPresent = false) = 0;
	protected:
		std::vector<Attachment> _attachments;
};

typedef std::shared_ptr<RenderPass> spRenderPass;

class PipelineInfo {
	friend class Pipeline;
	public:
		PipelineInfo();
		PipelineInfo(const PipelineInfo& r) = default;
		~PipelineInfo() = default;

		void inputAssembly(PrimitiveTopology topology);
		void viewport(const float& x,const float& y,const float& width,const float& height,const float& minDepth = 0.0f,const float& maxDepth = 1.0f);
		void viewport(const Viewport& view);

		void scissor(const glm::ivec2& offset,const glm::ivec2& size);

		void dynamicScissor();
		void dynamicViewport();

		void rasterizer(const PolygonMode& pmode = PolygonMode::Fill,
						const CullMode& cmode = CullMode::Back,
						const FrontFace& face = FrontFace::CW,
						const float& lineWidth = 1.0f,
						const bool& depthClamp = false,
						const bool& discard = false,
						const bool& depthBias = false,const glm::vec3& depthBiasFactor = glm::vec3(0.0f)); // depthBiasFactor x - depthBiasConstantFactor, y - depthBiasClamp, z - depthBiasSlopeFactor

		void multisampling(const SampleCount& count = SampleCount::s1,
						   const bool& sampleShading = false,
						   const float& minSampleShading = 1.0f,
						   const bool& alphaToCoverageEnable = false,
						   const bool& alphaToOneEnable = false);

		void blend(const uint& attachments = 1,const bool& enable = true,const std::vector<ColorComponent>& writeMask = {ColorComponent::eR,ColorComponent::eG,ColorComponent::eB,ColorComponent::eA},
					const BlendFactor& srcColorBlendFactor = BlendFactor::SrcAlpha,const BlendFactor& dstColorBlendFactor = BlendFactor::OneMinusSrcAlpha,
					const BlendOp& colorBlendOp = BlendOp::Add,
					const BlendFactor& srcAlphaBlendFactor = BlendFactor::One,const BlendFactor& dstAlphaBlendFactor = BlendFactor::Zero,
					const BlendOp& alphaBlendOp = BlendOp::Add);
		void depth(const bool& enable = true, const bool& write = true,const CompareOp& comp = CompareOp::Less);

		const PrimitiveTopology& getTopology() const;
		const Viewport& getViewport() const;
		const glm::ivec4& getScissor() const;

		const RasterizationState& getRasterizationState() const;
		const MultisamplingState& getMultisamplingState() const;
		const std::vector<BlendAttachmentState>& getBlendAttachments()const;
		const DepthState& getDepthState() const;

		const std::unordered_map<ShaderStage,std::string>& getShaders() const;
		const spRenderPass& getRenderPass() const;

		const std::vector<spDescSet>& getDescSets() const;

		bool isDynamicScissor() const;
		bool isDynamicViewport() const;

	 	void addShader(const ShaderStage& type,const std::string& filename); // Bascily import glsl
	 	void setRenderPass(const spRenderPass& rp);

	 	void setDescSet(const std::vector<spDescSet>& descSets);
	 	void setDescSet(const spDescSet& descSet);
	private:
		PrimitiveTopology _topology;
		Viewport          _viewport;
		glm::ivec4        _scissor;

		RasterizationState _rasterizer;
		MultisamplingState _multisampling;
		std::vector<BlendAttachmentState> _blendAttachments;
		DepthState  _depthStencil;

		bool _dynamicScissor;
		bool _dynamicViewport;

		std::unordered_map<ShaderStage,std::string> _shaders;
		spRenderPass _renderPass;
		std::vector<spDescSet> _descSets;
};

class Pipeline {
	public:
		explicit Pipeline(const PipelineInfo& rp){_renderPattern = rp;}
		virtual ~Pipeline() = default;

		PipelineInfo info(){return _renderPattern;}
	protected:
		PipelineInfo _renderPattern;
};

typedef std::shared_ptr<Pipeline> spPipeline;

};
