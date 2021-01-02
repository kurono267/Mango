//
// Created by kurono267 on 2019-07-29.
//

#include "Renderable.hpp"

using namespace mango;

namespace mango {

Renderable::Renderable(const spMesh &mesh, const spMaterial &material) : _mesh(mesh), _material(material) {

}

void Renderable::setMesh(const mango::spMesh &mesh) {
	_mesh = mesh;
}

void Renderable::setMaterial(const mango::spMaterial &material) {
	_material = material;
}

void Renderable::setSkin(const spSkin &skin) {
	_skin = skin;
}

spMesh Renderable::getMesh() {
	return _mesh;
}

spMaterial Renderable::getMaterial() {
	return _material;
}

spSkin Renderable::getSkin() {
	return _skin;
}

std::shared_ptr<Renderable> Renderable::create(const spMesh &mesh, const spMaterial &material) {
	return std::make_shared<Renderable>(mesh, material);
}

void Renderable::setRenderType(uint32_t renderType) {
	_renderType = renderType;
}

uint32_t Renderable::getRenderType() {
	return _renderType;
}

}