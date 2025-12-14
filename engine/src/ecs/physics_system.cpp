#include "engine/ecs/physcis_system.h"

#include "engine/ecs/colliders.h"
#include "engine/ecs/transform.h"

namespace engine::ecs {

size_t PhysicsSystem::PairHash::operator()(const std::pair<Entity, Entity>& p) const noexcept {
    return std::hash<Entity>{}(p.first) ^ (std::hash<Entity>{}(p.second) << 1);
}

PhysicsSystem::PhysicsSystem(
    const std::shared_ptr<EventBus>& eventBus,
    const std::shared_ptr<Time>& time
) :
    _eventBus(eventBus),
    _time(time) {

}

void PhysicsSystem::Update(World& world) {
    Move(world);
    Collisions(world);
}

void PhysicsSystem::Move(World& world) {
    world.ForEachWith<Transform, RigidBody>([this](Transform& t, const RigidBody& rb) {
       t.position += rb.velocity * _time->deltaTime;
    });
}

void PhysicsSystem::Collisions(World &world) {
    std::vector<std::tuple<Entity, Transform *, BoxCollider *>> colliders;

    world.ForEach<Transform, BoxCollider>(
            [&](Entity &e, Transform &t, BoxCollider &c) { colliders.emplace_back(e, &t, &c); });

    std::vector<std::tuple<Entity, Entity>> currentIntersects {};

    for (size_t i = 0; i < colliders.size(); ++i) {
        auto [a, ta, ca] = colliders[i];

        const glm::vec3 aMin = ta->position - ca->size * 0.5f;
        const glm::vec3 aMax = ta->position + ca->size * 0.5f;

        for (size_t j = i + 1; j < colliders.size(); ++j) {
            auto [b, tb, cb] = colliders[j];

            const glm::vec3 bMin = tb->position - cb->size * 0.5f;
            const glm::vec3 bMax = tb->position + cb->size * 0.5f;

            const bool intersects = (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
                                    (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
                                    (aMin.z <= bMax.z && aMax.z >= bMin.z);

            if (intersects)
                currentIntersects.emplace_back(a, b);
        }
    }

    std::unordered_set<std::pair<Entity, Entity>, PairHash> newLast;

    for (const auto& [a, b] : currentIntersects) {
        auto pair = MakePair(a, b);

        if (!lastSet.contains(pair))
            _eventBus->Emit<CollisionEvent>(a, b);

        newLast.insert(pair);
    }

    lastSet = std::move(newLast);
}

std::pair<Entity, Entity> PhysicsSystem::MakePair(Entity a, Entity b) {
    if (a < b)
        return {a, b};
    return {b, a};
}

}