//
// Created by kurono267 on 2019-07-29.
//

#include "Geometry.hpp"

using namespace mango;

Geometry::Geometry(const mango::spMesh &mesh, const mango::spMaterial &material) : _mesh(mesh), _material(material) {

}

void Geometry::setMesh(const mango::spMesh &mesh) {
	_mesh = mesh;
}

void Geometry::setMaterial(const mango::spMaterial &material) {
	_material = material;
}

spMesh mango::Geometry::getMesh() {
	return _mesh;
}

spMaterial mango::Geometry::getMaterial() {
	return _material;
}

std::shared_ptr<Geometry> Geometry::create(const spMesh &mesh, const spMaterial &material) {
	return std::make_shared<Geometry>(mesh,material);
}
