//
// Created by kurono267 on 06.07.18.
//

#include "Mesh.hpp"
#include "../api/CommandBuffer.hpp"
#include "../api/Instance.hpp"

using namespace mango;

void Mesh::create(const spDevice& device, size_t vertexBufferSize, size_t indexBufferSize, size_t vertexSize, size_t indexSize, MeshType type) {
	_vertexSize = vertexSize;
	_indexSize = indexSize;
	_type = type;

	_vbHost = device->createBuffer(BufferType::Vertex | BufferType::Storage,type==MeshType::Static?MemoryType::HOST:MemoryType::DEVICE_HOST,vertexBufferSize);

	_ibHost = device->createBuffer(BufferType::Index | BufferType::Storage,type==MeshType::Static?MemoryType::HOST:MemoryType::DEVICE_HOST,indexBufferSize);

	if(type == MeshType::Static) {
		_vbDevice = device->createBuffer(BufferType::Vertex | BufferType::Storage, MemoryType::DEVICE,
										 vertexBufferSize);
		_ibDevice = device->createBuffer(BufferType::Index | BufferType::Storage, MemoryType::DEVICE, indexBufferSize);
	} else {
		_vbDevice = _vbHost;
		_ibDevice = _ibHost;
	}
}

sVertex::sVertex(const glm::vec3& pos_,const glm::vec2& uv_,const glm::vec3& normal_)
: pos(pos_),uv(uv_),normal(normal_){}

sVertex::sVertex(const float& px,const float& py,const float& pz,const float& u,const float& v,const float& nx,const float& ny,const float& nz) :
	pos(px,py,pz),uv(u,v),normal(nx,ny,nz)
{}

void Mesh::draw(const spCommandBuffer& cmd, int indexCount, int instanceCount, int firstInstance){
	bind(cmd);
	cmd->drawIndexed(indexCount>=0?indexCount:indicesCount(),instanceCount>=0?instanceCount:1,firstInstance);
}

void Mesh::bind(const spCommandBuffer& cmd) {
	cmd->bindVertexBuffer(_vbDevice);
	if(_indexSize == 2)cmd->bindIndexBuffer16(_ibDevice);
	else cmd->bindIndexBuffer(_ibDevice);
}

void Mesh::updateVertices(const spCommandBuffer& cmd) {
	if(_type == MeshType::Static)_vbHost->copy(_vbDevice,cmd);
}

void Mesh::updateIndices(const spCommandBuffer& cmd) {
	if(_type == MeshType::Static)_ibHost->copy(_ibDevice,cmd);
}

void Mesh::updateBoundingBox() {
	_bbox = BBox();
	sVertex* vertices = mapVertices();
	int vertexCount = verticesCount();
	for(int i = 0;i<vertexCount;++i){
		const sVertex& vertex = vertices[i];
		_bbox.expand(vertex.pos);
	}
	unmapVertices();
}

BBox Mesh::getBoundingBox() {
	return _bbox;
}

size_t Mesh::verticesCount() {
	return _vbHost->size()/_vertexSize;
}

void Mesh::unmapVertices() {
	_vbHost->unmap();
}

size_t Mesh::indicesCount() {
	return _ibHost->size()/_indexSize;
}

void Mesh::unmapIndices() {
	_ibHost->unmap();
}

void Mesh::genNormals() {
	sVertex* vertices = mapVertices();
	uint32_t* indices = mapIndices();
	std::unordered_map<uint32_t,std::pair<glm::vec3,float>> normals;
	for(int i = 0;i<indicesCount();i+=3){
		sVertex& v0 = vertices[indices[i]];
		sVertex& v1 = vertices[indices[i+1]];
		sVertex& v2 = vertices[indices[i+2]];

		glm::vec3 normal = glm::normalize(glm::cross(v2.pos-v0.pos,v1.pos-v0.pos));
		for(int v = 0;v<3;++v){
			auto normalItr = normals.find(indices[i]);
			if(normalItr == normals.end()){
				normals.emplace(indices[i],std::pair<glm::vec3,float>(normal,1.f));
			} else {
				glm::vec3& currNormal = normalItr->second.first;
				float& countNormals = normalItr->second.second;
				currNormal = (currNormal*countNormals+normal)/(countNormals+1.f);
				countNormals++;
			}
		}
	}

	for(int i = 0;i<indicesCount();i++) {
		vertices[indices[i]].normal = normals[indices[i]].first;
	}
	unmapVertices();
	unmapIndices();

	updateVertices();
	updateIndices();
}

size_t Mesh::vertexBufferSize() {
	return _vbHost->size();
}

size_t Mesh::indexBufferSize() {
	return _ibHost->size();
}

spMesh mango::createQuad(const glm::vec2& uvScale){
	auto device = Instance::device();

	std::vector<sVertex> vdata = {
			sVertex(-1.0, 1.0, 0.0f, // Pos
					0.0f, 0.0f,
					0.f,0.f,1.f), 	 // Texcoord
			sVertex(-1.0, -1.0, 0.0f,// Pos
					0.0f, uvScale.y,
					0.f,0.f,1.f),     // Texcoord
			sVertex(1.0, -1.0, 0.0f, // Pos
					uvScale.x, uvScale.y,
					0.f,0.f,1.f),       // Texcoord
			sVertex(1.0, 1.0, 0.0f,  // Pos
					uvScale.x, 0.0f,
					0.f,0.f,1.f)};     // Texcoord

	std::vector<uint32_t> idata = {0,1,2,/*First*/2,3,0/*Second triangle*/};

	auto mesh = std::make_shared<Mesh>();
	mesh->create(device,vdata,idata);
	return mesh;
}

spMesh mango::createCube(){
	auto device = Instance::device();

	std::vector<sVertex> vdata(24);

	// front
	vdata[0] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(0.0f, 1.0f), glm::vec3(0,0,1));
	vdata[1] = sVertex(glm::vec3(1.0, -1.0,  1.0),glm::vec2(1.0f, 1.0f),glm::vec3(0,0,1));
	vdata[2] = sVertex(glm::vec3(1.0,  1.0,  1.0),glm::vec2(1.0f, 0.0f),glm::vec3(0,0,1));
	vdata[3] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(0.0f, 0.0f),glm::vec3(0,0,1));
	// top
	vdata[4] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(0.0f, 1.0f),glm::vec3(0,1,0));
	vdata[5] = sVertex(glm::vec3(1.0,  1.0,  1.0),glm::vec2(1.0f, 1.0f),glm::vec3(0,1,0));
	vdata[6] = sVertex(glm::vec3(1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f),glm::vec3(0,1,0));
	vdata[7] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f),glm::vec3(0,1,0));
	// back
	vdata[8] = sVertex(glm::vec3(1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f),glm::vec3(0,0,-1));
	vdata[9] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f),glm::vec3(0,0,-1));
	vdata[10] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f),glm::vec3(0,0,-1));
	vdata[11] = sVertex(glm::vec3(1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f),glm::vec3(0,0,-1));
	// bottom
	vdata[12] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f),glm::vec3(0,-1,0));
	vdata[13] = sVertex(glm::vec3( 1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f),glm::vec3(0,-1,0));
	vdata[14] = sVertex(glm::vec3( 1.0, -1.0,  1.0),glm::vec2(1.0f, 0.0f),glm::vec3(0,-1,0));
	vdata[15] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(0.0f, 0.0f),glm::vec3(0,-1,0));
	// left
	vdata[16] = sVertex(glm::vec3(-1.0, -1.0, -1.0),glm::vec2(0.0f, 1.0f),glm::vec3(-1,0,0));
	vdata[17] = sVertex(glm::vec3(-1.0, -1.0,  1.0),glm::vec2(1.0f, 1.0f),glm::vec3(-1,0,0));
	vdata[18] = sVertex(glm::vec3(-1.0,  1.0,  1.0),glm::vec2(1.0f, 0.0f),glm::vec3(-1,0,0));
	vdata[19] = sVertex(glm::vec3(-1.0,  1.0, -1.0),glm::vec2(0.0f, 0.0f),glm::vec3(-1,0,0));
	// right
	vdata[20] = sVertex(glm::vec3( 1.0, -1.0,  1.0),glm::vec2(0.0f, 1.0f),glm::vec3(-1,0,0));
	vdata[21] = sVertex(glm::vec3( 1.0, -1.0, -1.0),glm::vec2(1.0f, 1.0f),glm::vec3(-1,0,0));
	vdata[22] = sVertex(glm::vec3( 1.0,  1.0, -1.0),glm::vec2(1.0f, 0.0f),glm::vec3(-1,0,0));
	vdata[23] = sVertex(glm::vec3( 1.0,  1.0,  1.0),glm::vec2(0.0f, 0.0f),glm::vec3(-1,0,0));

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

spMesh mango::createSphere(const int segments) {
	auto device = Instance::device();

	std::vector<sVertex> vertices;
	for(int y = 0;y<=segments;++y){
		for(int x = 0;x<=segments;++x){
			glm::vec3 pos;
			float angle0 = M_PI/2.f-((float)y/(float)segments)*M_PI;
			float angle1 = ((float)x/(float)segments)*2.f*M_PI;

			pos.x = cos(angle0)*cos(angle1);
			pos.y = cos(angle0)*sin(angle1);
			pos.z = sin(angle0);

			sVertex v;
			v.pos = pos;
			v.normal = glm::normalize(pos);
			v.uv = glm::vec2((float)x/(float)segments,(float)y/(float)segments);
			vertices.push_back(v);
		}
	}

	std::vector<uint32_t> indices;
	for(int y = 0;y<segments;++y){
		uint32_t k1 = y * (segments + 1);     // beginning of current stack
		uint32_t k2 = k1 + segments + 1;      // beginning of next stack
		for(int x = 0;x<segments;++x){
			if(y != 0) {
				indices.push_back(k1+x);
				indices.push_back(k2+x);
				indices.push_back(k1 +x+ 1);
			}

			// k1+1 => k2 => k2+1
			if(y != (segments-1)){
				indices.push_back(k1 +x+ 1);
				indices.push_back(k2+x);
				indices.push_back(k2 +x+ 1);
			}
		}
	}

	auto mesh = std::make_shared<Mesh>();
	mesh->create(device,vertices,indices);
	//mesh->genNormals();
	return mesh;
}

