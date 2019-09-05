//
// Created by kurono267 on 06.07.18.
//

#include "Mesh.hpp"
#include "api/CommandBuffer.hpp"

using namespace mango;

void Mesh::create(const mango::spDevice &device, const std::vector<mango::sVertex> &vertices,
                         const std::vector<uint32_t> &indices) {
	_bbox = BBox();
	for(auto& vertex : vertices){
		_bbox.expand(vertex.pos);
	}
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

BBox Mesh::getBoundingBox() {
	return _bbox;
}

size_t Mesh::verticesCount() {
	return _vbHost->size()/sizeof(sVertex);
}

sVertex *Mesh::mapVertices() {
	return (sVertex*)_vbHost->map();
}

void Mesh::unmapVertices() {
	_vbHost->unmap();
}

size_t Mesh::indicesCount() {
	return _ibHost->size()/sizeof(uint32_t);
}

uint32_t *Mesh::mapIndices() {
	return (uint32_t*)_ibHost->map();
}

void Mesh::unmapIndices() {
	_ibHost->unmap();
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

spMesh mango::createCube(const mango::spDevice& device){
	std::vector<sVertex> vdata(24);

	// front
	vdata[0] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(0.0f, 1.0f));
	vdata[1] = sVertex(glm::vec3(1.0, -1.0,  1.0),glm::vec2(1.0f, 1.0f));
	vdata[2] = sVertex(glm::vec3(1.0,  1.0,  1.0),glm::vec2(1.0f, 0.0f));
	vdata[3] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(0.0f, 0.0f));
	// top
	vdata[4] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(0.0f, 1.0f));
	vdata[5] = sVertex(glm::vec3(1.0,  1.0,  1.0),glm::vec2(1.0f, 1.0f));
	vdata[6] = sVertex(glm::vec3(1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f));
	vdata[7] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f));
	// back
	vdata[8] = sVertex(glm::vec3(1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f));
	vdata[9] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f));
	vdata[10] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f));
	vdata[11] = sVertex(glm::vec3(1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f));
	// bottom
	vdata[12] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f));
	vdata[13] = sVertex(glm::vec3( 1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f));
	vdata[14] = sVertex(glm::vec3( 1.0, -1.0,  1.0),glm::vec2(1.0f, 0.0f));
	vdata[15] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(0.0f, 0.0f));
	// left
	vdata[16] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f));
	vdata[17] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(1.0f, 1.0f));
	vdata[18] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(1.0f, 0.0f));
	vdata[19] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f));
	// right
	vdata[20] = sVertex(glm::vec3( 1.0, -1.0,  1.0),glm::vec2(0.0f, 1.0f));
	vdata[21] = sVertex(glm::vec3( 1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f));
	vdata[22] = sVertex(glm::vec3( 1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f));
	vdata[23] = sVertex(glm::vec3( 1.0,  1.0,  1.0),glm::vec2(0.0f, 0.0f));

	std::vector<uint32_t> idata = {
			0,  1,  2,
			2,  3,  0,
			4,  5,  6,
			6,  7,  4,
			8,  9, 10,
			10, 11,  8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
	};

	auto mesh = std::make_shared<Mesh>();
	mesh->create(device,vdata,idata);
	return mesh;
}

