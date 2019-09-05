//
// Created by kurono267 on 2019-09-05.
//

#include <libshaderc/shaderc.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "ShaderVK.hpp"
#include <api/Utils.hpp>
#include "DeviceVK.hpp"

using namespace mango;
using namespace mango::vulkan;

class GLSLIncluder : public shaderc::CompileOptions::IncluderInterface {
	public:
		GLSLIncluder() = default;
		~GLSLIncluder() override;

		// Resolves a requested source file of a given type from a requesting
		// source into a shaderc_include_result whose contents will remain valid
		// until it's released.
		shaderc_include_result* GetInclude(const char* requested_source,
										   shaderc_include_type type,
										   const char* requesting_source,
										   size_t include_depth) override;
		// Releases an include result.
		void ReleaseInclude(shaderc_include_result* include_result) override;

		// Returns a reference to the member storing the set of included files.
		const std::unordered_set<std::string>& file_path_trace() const {
			return included_files_;
		}

	private:
		// The full path and content of a source file.
		struct FileInfo {
			const std::string full_path;
			std::string contents;
		};

		// The set of full paths of included files.
		std::unordered_set<std::string> included_files_;
};


shaderc_include_result* MakeErrorIncludeResult(const char* message) {
	return new shaderc_include_result{"", 0, message, strlen(message)};
}

GLSLIncluder::~GLSLIncluder() = default;

shaderc_include_result* GLSLIncluder::GetInclude(
		const char* requested_source, shaderc_include_type include_type,
		const char* requesting_source, size_t) {
	//std::cout << "requested_source: " << std::endl;
	//std::cout << requested_source << std::endl;
	//std::cout << "requesting_source: " << std::endl;
	//std::cout << requesting_source << std::endl;

	using namespace std::filesystem;

	const std::string full_path =
			(include_type == shaderc_include_type_relative)
			? path(requesting_source).parent_path()/requested_source
			: requested_source;

	if (full_path.empty())
		return MakeErrorIncludeResult("Cannot find or open include file.");

	//std::cout << "full_path " << full_path << std::endl;
	FileInfo* new_file_info = new FileInfo{full_path, {}};
	new_file_info->contents = mango::readFile(full_path);
	if(new_file_info->contents.empty()){
		return MakeErrorIncludeResult("Cannot read file");
	}

	included_files_.insert(full_path);

	return new shaderc_include_result{
			new_file_info->full_path.data(), new_file_info->full_path.length(),
			new_file_info->contents.data(), new_file_info->contents.size(),
			new_file_info};
}

void GLSLIncluder::ReleaseInclude(shaderc_include_result* include_result) {
	FileInfo* info = static_cast<FileInfo*>(include_result->user_data);
	delete info;
	delete include_result;
}

std::optional<vk::ShaderModule> ShaderVK::createShader(const mango::ShaderStage &type, const std::string &filename) {
	std::string content = mango::readFile(filename);
	shaderc::Compiler shaderCompiler;
	shaderc::CompileOptions compileOptions;
	compileOptions.SetIncluder(std::make_unique<GLSLIncluder>());

	shaderc_shader_kind shader_kind;
	switch (type){
		case mango::ShaderStage::Vertex: shader_kind = shaderc_vertex_shader;
			break;
		case mango::ShaderStage::Fragment: shader_kind = shaderc_fragment_shader;
			break;
		case mango::ShaderStage::Compute: shader_kind = shaderc_compute_shader;
			break;
		case mango::ShaderStage::Geometry: shader_kind = shaderc_geometry_shader;
			break;
		case mango::ShaderStage::TessellationControl: shader_kind = shaderc_tess_control_shader;
			break;
		case mango::ShaderStage::TessellationEvaluation: shader_kind = shaderc_tess_evaluation_shader;
			break;
		default:
			throw std::logic_error("Shader Stage not supported");
	}

	auto shaderResult = shaderCompiler.CompileGlslToSpv(content,shader_kind,filename.c_str(),compileOptions);
	int errors = shaderResult.GetNumErrors();
	if(errors != 0){
		std::cout << shaderResult.GetErrorMessage() << std::endl;
		return {};
	}
	std::vector<uint32_t> shaderBinary;
	for(auto b : shaderResult){
		shaderBinary.push_back(b);
	}

	vk::ShaderModuleCreateInfo createInfo(vk::ShaderModuleCreateFlags(),shaderBinary.size()*sizeof(uint32_t),shaderBinary.data());
	auto shaderModule = Instance::device<DeviceVK>()->getDevice().createShaderModule(createInfo);

	return shaderModule;
}
