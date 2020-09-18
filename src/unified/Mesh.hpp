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

enum class MeshType {
	Static,
	Dynamic
};

class Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		template<typename VertexType = sVertex, typename IndexType = uint32_t>
		void create(const spDevice& device, const std::vector<VertexType>& vertices,const std::vector<IndexType>& indices, MeshType type = MeshType::Static){
			_bbox = BBox();
			for(auto& vertex : vertices){
				_bbox.expand(vertex.pos);
			}

			create(device,vertices.size()*sizeof(VertexType),indices.size()*sizeof(IndexType),sizeof(VertexType),sizeof(IndexType),type);

			fillVertices(vertices);
			fillIndices(indices);

			updateVertices();
			updateIndices();
		}
		void create(const spDevice& device, size_t vertexBufferSize, size_t indexBufferSize, size_t vertexSize, size_t indexSize, MeshType type = MeshType::Static);
		void draw(const spCommandBuffer& cmd, int indexCount = -1, int instanceCount = -1, int firstInstance = 0);
		void bind(const spCommandBuffer& cmd);

		size_t vertexBufferSize();
		size_t verticesCount();
		template<typename VertexType = sVertex>
		VertexType* mapVertices() {
			return (VertexType*)_vbHost->map();
		}
		void unmapVertices();

		size_t indexBufferSize();
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

		void updateVertices(const spCommandBuffer& cmd = nullptr);
		void updateIndices(const spCommandBuffer& cmd = nullptr);

		BBox getBoundingBox();

		void genNormals(); // TODO need improve
	protected:
		spBuffer _vbHost;
		spBuffer _vbDevice;
		spBuffer _ibHost;
		spBuffer _ibDevice;
		size_t _vertexSize;
		size_t _indexSize;
		BBox _bbox;
		MeshType _type;
};

typedef std::shared_ptr<Mesh> spMesh;

spMesh createQuad(const glm::vec2& uvScale = glm::vec2(1,1));
spMesh createCube();
spMesh createSphere(const int segments);

};

#endif //MANGO_MESH_HPP
