//
// Created by kurono267 on 30.12.2020.
//

#pragma once

#include <entt/entt.hpp>
#include <iostream>

namespace mango {

class SceneNode;
typedef std::shared_ptr<SceneNode> spSceneNode;

/// Singleton class for contain entt::registry
class Context {
	public:
		static entt::registry& registry();
	private:
		Context() = default;
		~Context() = default;
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		static Context& get();
	private:
		entt::registry _registry;
};

class Entity {
	public:
		Entity();
		~Entity();

		static std::shared_ptr<Entity> create();

		template<typename Component>
		void addComponent(const Component& component) {
			//std::cout << "Entity " << (uint32_t)_entity << " Add component " << typeid(Component).name() << std::endl;
			Context::registry().emplace<Component>(_entity,component);
		}

		template<typename Component>
		void replaceComponent(const Component& component){
			Context::registry().emplace_or_replace<Component>(_entity,component);
		}

		template<typename Component>
		void removeComponent(){
			Context::registry().remove<Component>(_entity);
		}

		template<typename Component>
		Component& getComponent() {
			return Context::registry().get<Component>(_entity);
		}

		template<typename Component>
		bool hasComponent(){
			return Context::registry().has<Component>(_entity);
		}

		const entt::entity& entity() const;
	private:
		entt::entity _entity;
};

typedef std::shared_ptr<Entity> spEntity;

}

