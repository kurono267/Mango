#pragma once

#include "mango.hpp"
#include <unified/Mesh.hpp>
#include <algorithm>

#define NODE_MAX_TRIANGLE 1 // Max triangle in node

namespace mango {

struct BVHNode {
	BVHNode() {
		for (int i = 0; i < 4; ++i)triIds[i] = -1;
		data.isLeaf = false;
	}

	BVHNode(const BVHNode &n) : box(n.box), data(n.data) {}

	// Alligned BVH Node
	BBox box;

	// Data changes by leaf or not current node
	// If Leaf x,y,z,w index of triangle
	// If Not leaf x,y r leaf, l leaf, z - meshID, w - depth
	union {
		int32_t triIds[4];
		struct {
			int32_t right;
			int32_t left;
			float split;
			int32_t isLeaf;
		} data;
	};
};

std::ostream &operator<<(std::ostream &os, const BVHNode &n);

class BVH {
	public:
		struct Prim {
			Prim();

			glm::vec3 minBox;
			glm::vec3 maxBox;
			glm::vec3 center;

			uint32_t id;
			uint32_t mortonCode;
		};

		BVH(const mango::spMesh &mesh);

		virtual ~BVH();

		std::vector<BVHNode> &nodes();

		size_t rootID();

		Uniform getBuffer();
	protected:
		void recursiveLBVH(BVHNode &root, const std::vector<Prim> &primitives, uint32_t start, uint32_t end, int depth,
						   uint mesh_id);

		uint32_t findSplitLBVH(const std::vector<Prim> &primitives, uint32_t start, uint32_t end);

		size_t rootId;
		std::vector<BVHNode> _nodes;
		size_t _maxDepth;
		Uniform _storageBuffer;
};

typedef std::shared_ptr<BVH> spBVH;

}
