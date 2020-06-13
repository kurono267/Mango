#include "BVH.hpp"
#include "BBox.hpp"
#include "../api/Instance.hpp"

using namespace glm;
using namespace mango;

// Expands a 10-bit integer into 30 bits
// by inserting 2 zeros after each bit.
uint32_t expandBits(uint32_t v)
{
	v = (v * 0x00010001u) & 0xFF0000FFu;
	v = (v * 0x00000101u) & 0x0F00F00Fu;
	v = (v * 0x00000011u) & 0xC30C30C3u;
	v = (v * 0x00000005u) & 0x49249249u;
	return v;
}

// Calculates a 30-bit Morton code for the
// given 3D point located within the unit cube [0,1].
const glm::vec3 maxMorton(1023.f);
const glm::vec3 minMorton(0.f);
const glm::vec3 sizeMorton(1024.f);
uint32_t morton3D(glm::vec3 v)
{
	v = glm::clamp(v*sizeMorton,minMorton,maxMorton);
	uint32_t xx = expandBits((uint32_t)v.x);
	uint32_t yy = expandBits((uint32_t)v.y);
	uint32_t zz = expandBits((uint32_t)v.z);
	return xx * 4 + yy * 2 + zz;
}

BVH::BVH(const spMesh& mesh) {
	// Construct primitives list
	size_t vertexCount = mesh->verticesCount();
	size_t indexCount = mesh->indicesCount();
	sVertex* vertices = mesh->mapVertices();
	uint32_t* indices = mesh->mapIndices();
	BBox meshBox = mesh->getBoundingBox();

	std::vector<Prim> primitives;
	for(int i = 0;i<indexCount;i+=3){
		Prim prim;
		prim.center = glm::vec3(0.f);
		for(int v = 0;v<3;++v){
			auto index = indices[i+v];
			auto vPos = vertices[index].pos;
			prim.minBox = glm::min(prim.minBox,vPos);
			prim.maxBox = glm::max(prim.maxBox,vPos);
			prim.center += vPos;
		}
		prim.id = i;

		prim.center /= 3.f;
		auto normCenter = (prim.center-glm::vec3(meshBox.min))/glm::vec3(meshBox.max-meshBox.min);
		prim.mortonCode = morton3D(normCenter);

		primitives.push_back(prim);
	}
	mesh->unmapVertices();
	mesh->unmapIndices();
	// Sort primitives by morton code
	std::sort(primitives.begin(),primitives.end(),[](const auto& primA, const auto& primB){
		return primA.mortonCode < primB.mortonCode;
	});
	// Create root node
	BVHNode root;
	_nodes.emplace_back();
	rootId = _nodes.size()-1;
	_maxDepth = 0;

	recursiveLBVH(root,primitives,0,primitives.size()-1,0,0);

	_nodes[rootId] = root;

	_storageBuffer.create(Instance::device(),sizeof(BVHNode)*_nodes.size(),_nodes.data());
}

BVH::~BVH(){}

std::ostream& mango::operator<<(std::ostream& os, const BVHNode& n){
	os << "NODE" << std::endl;
	os << "MIN:" << glm::to_string(n.box.min) << std::endl;
	os << "MAX:" << glm::to_string(n.box.max) << std::endl;
	return os;
}

inline float SurfaceArea(const BVHNode& n){
	glm::vec3 sides(n.box.max-n.box.min);
	return 2*(sides.x*sides.y+sides.y*sides.z+sides.x*sides.z);
}

BVH::Prim::Prim() {
    minBox = glm::vec3(std::numeric_limits<float>::infinity());
    maxBox = glm::vec3(-std::numeric_limits<float>::infinity());
}

std::vector<BVHNode>& BVH::nodes(){
	return _nodes;
}

size_t BVH::rootID(){
	return rootId;
}

uint32_t BVH::findSplitLBVH(const std::vector<Prim>& primitives,uint32_t start, uint32_t end){
    auto startMorton = primitives[start].mortonCode;
    auto endMorton = primitives[end].mortonCode;

    if(startMorton == endMorton) // If morton code identical split is middle
        return (start+end) >> 1;  // (start+end)/2

    auto commonPrefix = std::__libcpp_clz(startMorton ^ endMorton);

    auto split = start; // initial guess
    auto step = end - start;

    do {
        step = (step + 1) >> 1; // exponential decrease
        auto newSplit = split + step; // proposed new position

        if (newSplit < end) {
            auto splitCode = primitives[newSplit].mortonCode;
            auto splitPrefix = std::__libcpp_clz(startMorton ^ splitCode);
            if (splitPrefix > commonPrefix)
                split = newSplit; // accept proposal
        }
    }
    while (step > 1);

    return split;
}

void BVH::recursiveLBVH(BVHNode& root, const std::vector<Prim>& primitives,uint32_t start, uint32_t end, int depth, uint mesh_id){
    if(_maxDepth < depth)_maxDepth = depth;
    BBox aabb;
    BBox centroid;
    // Recompute BBox
    for(int i = start;i<=end;++i){
        const Prim& p = primitives[i];
        aabb.expand(p.minBox,p.maxBox);
        centroid.expand(p.center);
    }
    root.box = aabb;

    uint32_t size = end-start+1;
    if(size <= NODE_MAX_TRIANGLE){
        root.data.isLeaf = true;
        for(uint32_t i = 0;i<NODE_MAX_TRIANGLE;++i){
            if(i < size){
                root.triIds[i] = primitives[start+i].id;
            } else root.triIds[i] = -1;
        }
        return;
    }

    auto split = findSplitLBVH(primitives, start, end);

    // Right node
    BVHNode left;
    _nodes.push_back(left);
    root.data.left = _nodes.size()-1;
    BVHNode right;
    _nodes.push_back(right);
    root.data.right = _nodes.size() - 1;

    recursiveLBVH(left,primitives,start,split,depth+1,mesh_id);
    recursiveLBVH(right, primitives, split+1, end, depth + 1, mesh_id);
    _nodes[root.data.left] = left;
    _nodes[root.data.right] = right;

    // Fill additional data to node
    root.data.split  = split;
}

Uniform BVH::getBuffer() {
	return _storageBuffer;
}



