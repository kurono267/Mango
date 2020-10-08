//
// Created by kurono267 on 2019-07-29.
//

#include "Geometry.hpp"

using namespace mango;

namespace mango {

Geometry::Geometry(const spMesh &mesh, const spMaterial &material) : _mesh(mesh), _material(material) {

}

void Geometry::setMesh(const mango::spMesh &mesh) {
	_mesh = mesh;
}

void Geometry::setMaterial(const mango::spMaterial &material) {
	_material = material;
}

void Geometry::setSkin(const spSkin &skin) {
	_skin = skin;
}

spMesh Geometry::getMesh() {
	return _mesh;
}

spMaterial Geometry::getMaterial() {
	return _material;
}

spSkin Geometry::getSkin() {
	return _skin;
}

std::shared_ptr<Geometry> Geometry::create(const spMesh &mesh, const spMaterial &material) {
	return std::make_shared<Geometry>(mesh, material);
}

}