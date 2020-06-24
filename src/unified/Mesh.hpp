//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_MESH_HPP
#define MANGO_MESH_HPP

#include <vector>
#include "../scene/BBox.hpp"
#include "../api/Buffer.hpp"
#include "../api/default.hpp"
#include "../api/Types.hpp"
#include "../api/Device.hpp"
#include "Uniform.hpp"

namespace mango {

class Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		void create(const spDevice& device, const std::vector<sVertex>& vertices,const std::vector<uint32_t>& indices);
		void draw(const spCommandBuffer& cmd);

		size_t verticesCount();
		sVertex* mapVertices();
		void unmapVertices();

		size_t indicesCount();
		uint32_t* mapIndices();
		void unmapIndices();

		BBox getBoundingBox();

		Uniform getVertexBuffer();
		Uniform getIndexBuffer();

		void genNormals(); // TODO need improve
	protected:
		spBuffer _vbHost;
		spBuffer _vbDevice;
		spBuffer _ibHost;
		spBuffer _ibDevice;
		uint32_t _indexCount;
		BBox _bbox;
};

typedef std::shared_ptr<Mesh> spMesh;

spMesh createQuad();
spMesh createCube();
spMesh createSphere(const int segments);

};

#endif //MANGO_MESH_HPP
