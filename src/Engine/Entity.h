#pragma once
#include <entt/entt.hpp>
#include <iostream> //print
#include <utility>
#include "Scene.h"
#include "Components.h"

class Entity {
public:
  Entity(entt::entity h, Scene* s)
    : handle(h), scene(s)
  {
    std::cout << "Entity Created";
  }

  ~Entity() {
    scene->r.destroy(handle);
    std::cout << "Entity Detroyed";
  }

  template<typename T>
  void addComponent(auto&&... args) {
    scene->r.emplace<T>(handle, std::forward<decltype(args)>(args)...);
  }

  template<typename T>
    void removeComponent() {
      scene->r.remove<T>(handle);
    }

private:
  entt::entity handle;
  Scene* scene;
};
