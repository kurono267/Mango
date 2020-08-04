//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_GEOMETRY_HPP
#define MANGO_GEOMETRY_HPP

#include "../unified/Mesh.hpp"
#include "Material.hpp"

namespace mango {

/// Geometry consist of mesh and material
class Geometry {
	public:
		/// Default constructor
		Geometry() = default;
		/// Construct Geometry with Mesh and Material
		/// @param mesh - Mesh
		/// @param material - Material
		Geometry(const spMesh& mesh, const spMaterial& material);

		/// Set mesh
		/// @param mesh - Mesh
		void setMesh(const spMesh& mesh);
		/// Set material
		/// @param material - Material
		void setMaterial(const spMaterial& material);

		/// Get mesh
		spMesh getMesh();
		/// Get material
		spMaterial getMaterial();

		/// Construct shared pointer of Geometry
		/// @param mesh - Mesh
		/// @param material - Material
		static std::shared_ptr<Geometry> create(const spMesh& mesh = nullptr, const spMaterial& material = nullptr);
	protected:
		spMesh _mesh;
		spMaterial _material;
};

typedef std::shared_ptr<Geometry> spGeometry;

}

#endif //MANGO_GEOMETRY_HPP
