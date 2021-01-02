//
// Created by kurono267 on 30.12.2020.
//

#include "Context.hpp"

using namespace mango;

Context &Context::get() {
	static Context instance;
	return instance;
}

entt::registry& Context::registry() {
	Context& instance = get();
	return instance._registry;
}

Entity::Entity() {
	_entity = Context::registry().create();
}

std::shared_ptr<Entity> Entity::create() {
	return std::make_shared<Entity>();
}

const entt::entity& Entity::entity() const {
	return _entity;
}

Entity::~Entity() {
	Context::registry().remove_all(_entity);
}
