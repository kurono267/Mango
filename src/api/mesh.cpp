//
// Created by kurono267 on 06.07.18.
//

#include "mesh.hpp"

using namespace mango;

void Mesh::create(const mango::spDevice &device, const std::vector<mango::sVertex> &vertices,
                         const std::vector<uint32_t> &indices) {
	_vb = device->createBuffer(BufferType::Vertex,vertices.size()*sizeof(sVertex), (void*)vertices.data());
	_ib = device->createBuffer(BufferType::Index,indices.size()*sizeof(uint32_t),(void*)vertices.data());
}

sVertex::sVertex(const glm::vec3& pos_,const glm::vec2& uv_,const glm::vec3& normal_)
: pos(pos_),uv(uv_),normal(normal_){}

sVertex::sVertex(const float& px,const float& py,const float& pz,const float& u,const float& v,const float& nx,const float& ny,const float& nz) :
	pos(px,py,pz),uv(u,v),normal(nx,ny,nz)
{}
