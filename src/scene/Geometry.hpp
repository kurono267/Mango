//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_GEOMETRY_HPP
#define MANGO_GEOMETRY_HPP

#include <unified/Mesh.hpp>
#include "Material.hpp"

namespace mango {

/// Geometry consist of mesh and material
class Geometry {
	public:
		Geometry() = default;
		Geometry(const spMesh& mesh, const spMaterial& material);

		void setMesh(const spMesh& mesh);
		void setMaterial(const spMaterial& material);

		spMesh getMesh();
		spMaterial getMaterial();

		static std::shared_ptr<Geometry> make(const spMesh& mesh = nullptr, const spMaterial& material = nullptr);
	protected:
		spMesh _mesh;
		spMaterial _material;
};

typedef std::shared_ptr<Geometry> spGeometry;

}

#endif //MANGO_GEOMETRY_HPP
