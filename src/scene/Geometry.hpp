//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_GEOMETRY_HPP
#define MANGO_GEOMETRY_HPP

#include <unified/Mesh.hpp>

namespace mango {

/// Geometry consist of mesh and material
class Geometry {
	public:

	protected:
		spMesh _mesh;
		spMaterial _material;
};

};

#endif //MANGO_GEOMETRY_HPP
