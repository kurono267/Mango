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

		template<typename VertexType = sVertex, typename IndexType = uint32_t>
		void create(const spDevice& device, const std::vector<VertexType>& vertices,const std::vector<IndexType>& indices){
			_bbox = BBox();
			for(auto& vertex : vertices){
				_bbox.expand(vertex.pos);
			}

			create(device,vertices.size()*sizeof(VertexType),indices.size()*sizeof(IndexType),sizeof(VertexType),sizeof(IndexType));

			fillVertices(vertices);
			fillIndices(indices);

			updateVertices();
			updateIndices();
		}
		void create(const spDevice& device, size_t vertexBufferSize, size_t indexBufferSize, size_t vertexSize, size_t indexSize);
		void draw(const spCommandBuffer& cmd, int indexCount = -1);
		void bind(const spCommandBuffer& cmd);

		size_t verticesCount();
		template<typename VertexType = sVertex>
		VertexType* mapVertices() {
			return (VertexType*)_vbHost->map();
		}
		void unmapVertices();

		size_t indicesCount();
		template<typename IndexType = uint32_t>
		IndexType* mapIndices() {
			return (IndexType*)_ibHost->map();
		}
		void unmapIndices();

		template<typename VertexType>
		void fillVertices(const std::vector<VertexType>& vertices, size_t offset = 0) {
			auto* data = mapVertices<VertexType>();
			memcpy(data+offset,vertices.data(),vertices.size()*sizeof(VertexType));
			unmapVertices();
		}

		template<typename IndexType>
		void fillIndices(const std::vector<IndexType>& indices, size_t offset = 0) {
			auto* data = mapIndices<IndexType>();
			memcpy(data+offset,indices.data(),indices.size()*sizeof(IndexType));
			unmapIndices();
		}

		void updateVertices();
		void updateIndices();

		BBox getBoundingBox();

		Uniform getVertexBuffer();
		Uniform getIndexBuffer();

		void genNormals(); // TODO need improve
	protected:
		spBuffer _vbHost;
		spBuffer _vbDevice;
		spBuffer _ibHost;
		spBuffer _ibDevice;
		size_t _vertexSize;
		size_t _indexSize;
		BBox _bbox;
};

typedef std::shared_ptr<Mesh> spMesh;

spMesh createQuad();
spMesh createCube();
spMesh createSphere(const int segments);

};

#endif //MANGO_MESH_HPP
