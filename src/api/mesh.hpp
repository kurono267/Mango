//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_MESH_HPP
#define MANGO_MESH_HPP

#include <vector>
#include "buffer.hpp"
#include "default.hpp"
#include "types.hpp"
#include "device.hpp"

namespace mango {

struct VertexAttrib {
	VertexAttrib() = default;
	VertexAttrib(const uint32_t& l,const uint32_t& b,const Format& f,const uint32_t& o) : location(l), binding(b), format(f), offset(o){}
	uint32_t location;
	uint32_t binding;
	Format format;
	uint32_t offset;
};

struct VertexBinding {
	VertexBinding(const uint32_t& b,const uint32_t& s) : binding(b), stride(s) {}
	uint32_t binding;
	uint32_t stride;
};

struct sVertex {
	sVertex(const glm::vec3& pos_ = glm::vec3(0.0f),const glm::vec2& uv_ = glm::vec2(0.0f),const glm::vec3& normal_ = glm::vec3(0.0f));
	sVertex(const float& px,const float& py,const float& pz,const float& u = 0.0f,const float& v = 0.0f,const float& nx = 0.0f,const float& ny = 0.0f,const float& nz = 0.0f);

	/*alignas(16)*/ glm::vec3 pos;
	/*alignas(16)*/ glm::vec3 normal;
	/*alignas(16)*/ glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 binormal;

	static VertexBinding bindingDesc(){
		return VertexBinding(0,sizeof(sVertex));
	}

	static std::vector<VertexAttrib> attributes(){
		std::vector<VertexAttrib> attrs(5);
		attrs[0] = VertexAttrib(0,0,Format::R32G32B32Sfloat,offsetof(sVertex,pos));
		attrs[1] = VertexAttrib(1,0,Format::R32G32B32Sfloat,offsetof(sVertex,normal));
		attrs[2] = VertexAttrib(2,0,Format::R32G32Sfloat,offsetof(sVertex,uv));
		attrs[3] = VertexAttrib(3,0,Format::R32G32B32Sfloat,offsetof(sVertex,tangent));
		attrs[4] = VertexAttrib(4,0,Format::R32G32B32Sfloat,offsetof(sVertex,binormal));
		return attrs;
	}
};

class Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		void create(const spDevice& device, const std::vector<sVertex>& vertices,const std::vector<uint32_t>& indices);
		void draw(const spCommandBuffer& cmd);
	protected:
		spBuffer _vb;
		spBuffer _ib;
		uint32_t _indexCount;
};

typedef std::shared_ptr<Mesh> spMesh;

spMesh createQuad(const mango::spDevice &device);

};

#endif //MANGO_MESH_HPP
