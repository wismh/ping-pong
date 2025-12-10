#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace engine::ecs {

using Entity = std::uint32_t;
class World;

class IComponentPool {
public:
  virtual ~IComponentPool() = default;

  virtual void DestroyEntity(Entity) = 0;
  virtual bool HasEntity(Entity) = 0;
};

template <typename Component>
class ComponentPool final : public IComponentPool {
public:
  Component& AttachComponent(const Entity& entity, Component& component) noexcept {
    size_t index = _components.size();
    _components.push_back(component);

    _entityToIndex.insert({entity, index});
    _indexToEntity.insert({index, entity});

    return component;
  }

  void DestroyEntity(const Entity entity) override {
    if (!_entityToIndex.contains(entity))
      return;

    size_t index = _entityToIndex[entity];
    _components.erase(_components.begin() + index);
    _entityToIndex.erase(_entityToIndex.find(entity));
    _indexToEntity.erase(_indexToEntity.find(index));
  }

  bool HasEntity(const Entity entity) override {
    return _entityToIndex.contains(entity);
  }

  Component& GetByEntity(const Entity entity) {
    if (!_entityToIndex.contains(entity))
      throw std::invalid_argument("ComponentPool doesn't have an entity");

    return _components[_entityToIndex[entity]];
  }

private:
  std::vector<Component> _components;
  std::map<Entity, size_t> _entityToIndex;
  std::map<size_t, Entity> _indexToEntity;
};

class ISystem {
public:
  virtual ~ISystem() = default;
  virtual void Update(World&) {};
};

class World final {
public:
  Entity CreateEntity() {
    Entity newEntity;

    if (_availableIds.empty()) {
      newEntity = _lastEntity++;
    } else {
      newEntity = _availableIds.back();
      _availableIds.pop_back();
    }

    _entities.push_back(newEntity);

    return newEntity;
  }

  template <typename Component>
  Component& AttachComponent(Entity entity, Component component) {
    const char* type = typeid(Component).name();

    if (!_components.contains(type))
      _components.insert(std::make_pair(type, std::make_shared<ComponentPool<Component>>()));

    return std::static_pointer_cast<ComponentPool<Component>>(_components[type])->AttachComponent(entity, component);
  }

  template <typename Component>
  bool HasComponent(const Entity entity) {
    const char* type = typeid(Component).name();

    if (!_components.contains(type))
      return false;

    return _components[type]->HasEntity(entity);
  }

  template <typename Component>
  Component& GetComponent(Entity entity) {
    const char* type = typeid(Component).name();

    if (!_components.contains(type))
      throw std::invalid_argument("ComponentPool doesn't have an entity");

    return std::static_pointer_cast<ComponentPool<Component>>(_components[type])->getByEntity(entity);
  }

  template <typename Func>
  void ForEach(Func function) {
    for (const auto& entity : _entities)
      function(entity);
  }

  template <typename... Components, typename Func>
  void ForEach(Func function) {
    for (const auto& entity : _entities)
      if ((_components[typeid(Components).name()]->HasEntity(entity) && ...))
        function(entity, std::static_pointer_cast<ComponentPool<Components>>(_components[typeid(Components).name()])->GetByEntity(entity)...);
  }

  template <typename... Components, typename Func>
  void ForEachWith(Func function) {
    for (const auto& entity : _entities)
      if ((_components[typeid(Components).name()]->HasEntity(entity) && ...))
        function(std::static_pointer_cast<ComponentPool<Components>>(_components[typeid(Components).name()])->GetByEntity(entity)...);
  }

  void AddSystem(std::unique_ptr<ISystem> system) {
    _systems.push_back(std::move(system));
  }

  void Update() {
    for (const auto& system : _systems)
      system->Update(*this);
  }

private:
  std::vector<Entity> _entities;
  std::vector<Entity> _availableIds;
  Entity _lastEntity = 0;

  std::map<const char*, std::shared_ptr<IComponentPool>> _components;
  std::vector<std::unique_ptr<ISystem>> _systems;
};

}