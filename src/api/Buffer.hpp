//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_BUFFER_HPP
#define MANGO_BUFFER_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <vector>
#include "Types.hpp"

namespace mango {

class CommandBuffer;

struct VertexAttrib {
	VertexAttrib() = default;
	VertexAttrib(const uint32_t& l,const uint32_t& b,const Format& f,const uint32_t& o) : location(l), binding(b), format(f), offset(o){}
	uint32_t location;
	uint32_t binding;
	Format format;
	uint32_t offset;
};

struct VertexBinding {
	VertexBinding() = default;
	VertexBinding(const uint32_t& b,const uint32_t& s) : binding(b), stride(s) {}
	uint32_t binding;
	uint32_t stride;
};

struct sVertex {
	sVertex(const glm::vec3& pos_ = glm::vec3(0.0f),const glm::vec2& uv_ = glm::vec2(0.0f),const glm::vec3& normal_ = glm::vec3(0.0f));
	sVertex(const float& px,const float& py,const float& pz,const float& u = 0.0f,const float& v = 0.0f,const float& nx = 0.0f,const float& ny = 0.0f,const float& nz = 0.0f);

	alignas(16) glm::vec3 pos;
	alignas(16) glm::vec3 normal;
	alignas(16) glm::vec2 uv;
	alignas(16) glm::vec3 tangent = glm::vec3(0.0f);
	alignas(16) glm::vec4 joints = glm::vec4(0.f);
	alignas(16) glm::vec4 weights = glm::vec4(0.f);

	static VertexBinding bindingDesc(){
		return VertexBinding(0,sizeof(sVertex));
	}

	static std::vector<VertexAttrib> attributes(){
		std::vector<VertexAttrib> attrs(6);
		attrs[0] = VertexAttrib(0,0,Format::R32G32B32Sfloat,offsetof(sVertex,pos));
		attrs[1] = VertexAttrib(1,0,Format::R32G32B32Sfloat,offsetof(sVertex,normal));
		attrs[2] = VertexAttrib(2,0,Format::R32G32Sfloat,offsetof(sVertex,uv));
		attrs[3] = VertexAttrib(3,0,Format::R32G32B32Sfloat,offsetof(sVertex,tangent));
		attrs[4] = VertexAttrib(4,0,Format::R32G32B32A32Sfloat,offsetof(sVertex,joints));
		attrs[5] = VertexAttrib(5,0,Format::R32G32B32A32Sfloat,offsetof(sVertex,weights));
		return attrs;
	}
};

class Buffer {
	public:
		Buffer() = default;
		virtual ~Buffer() = default;

		virtual void set(const size_t& size,const void* data) = 0;
		virtual void copy(const std::shared_ptr<Buffer>& dst, const std::shared_ptr<CommandBuffer>& cmd = nullptr) = 0;

		virtual size_t size() = 0;
		virtual void* map() = 0;
		virtual void unmap() = 0;
};

typedef std::shared_ptr<Buffer> spBuffer;

};

#endif //MANGO_BUFFER_HPP
