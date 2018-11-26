//
// Created by kurono267 on 06.07.18.
//

#include "mesh.hpp"
#include "commandbuffer.hpp"

using namespace mango;

void Mesh::create(const mango::spDevice &device, const std::vector<mango::sVertex> &vertices,
                         const std::vector<uint32_t> &indices) {
    _vbHost = device->createBuffer(BufferType::Vertex,MemoryType::HOST,vertices.size()*sizeof(sVertex),(void*)vertices.data());
    _vbDevice = device->createBuffer(BufferType::Vertex,MemoryType::DEVICE,vertices.size()*sizeof(sVertex));
    _vbHost->copy(_vbDevice);

	_ibHost = device->createBuffer(BufferType::Index,MemoryType::HOST,indices.size()*sizeof(uint32_t),(void*)indices.data());
    _ibDevice = device->createBuffer(BufferType::Index,MemoryType::DEVICE,indices.size()*sizeof(uint32_t));
    _ibHost->copy(_ibDevice);
	_indexCount = static_cast<uint32_t>(indices.size());
}

sVertex::sVertex(const glm::vec3& pos_,const glm::vec2& uv_,const glm::vec3& normal_)
: pos(pos_),uv(uv_),normal(normal_){}

sVertex::sVertex(const float& px,const float& py,const float& pz,const float& u,const float& v,const float& nx,const float& ny,const float& nz) :
	pos(px,py,pz),uv(u,v),normal(nx,ny,nz)
{}

void Mesh::draw(const spCommandBuffer& cmd){
	cmd->bindVertexBuffer(_vbDevice);
	cmd->bindIndexBuffer(_ibDevice);
	cmd->drawIndexed(_indexCount);
}

spMesh mango::createQuad(const mango::spDevice &device){
	std::vector<sVertex> vdata = {
			sVertex(-1.0, 1.0, 0.0f, // Pos
					0.0f, 0.0f), 	 // Texcoord
			sVertex(-1.0, -1.0, 0.0f,// Pos
					0.0f, 1.0f),     // Texcoord
			sVertex(1.0, -1.0, 0.0f, // Pos
					1.0, 1.0),       // Texcoord
			sVertex(1.0, 1.0, 0.0f,  // Pos
					1.0, 0.0f)};     // Texcoord

	std::vector<uint32_t> idata = {0,1,2,/*First*/2,3,0/*Second triangle*/};

	auto mesh = std::make_shared<Mesh>();
	mesh->create(device,vdata,idata);
	return mesh;
}
