#pragma once
#include "colliders.h"
#include "ecs.h"
#include "transform.h"
#include "rigidbody.h"
#include "../core/time.h"
#include "../core/event_bus.h"

namespace engine::ecs {

struct CollisionEvent {
    Entity a;
    Entity b;
};

class PhysicsSystem final : public ISystem {
    std::shared_ptr<EventBus> _eventBus;
    std::shared_ptr<Time> _time;

public:
    explicit PhysicsSystem(
        const std::shared_ptr<EventBus>& eventBus,
        const std::shared_ptr<Time>& time
    ) :
        _eventBus(eventBus),
        _time(time) {

    }

    void Update(World& world) override {
        Move(world);
        Collisions(world);
    }
private:
    void Move(World& world) {
        world.ForEachWith<Transform, RigidBody>([this](Transform& t, const RigidBody& rb) {
           t.position += rb.velocity * _time->deltaTime;
        });
    }

    void Collisions(World& world) const {
        std::vector<std::tuple<Entity, Transform*, BoxCollider*>> colliders;

        world.ForEach<Transform, BoxCollider>(
           [&](Entity& e, Transform& t, BoxCollider& c) {
               colliders.emplace_back(e, &t, &c);
           }
       );

        for (size_t i = 0; i < colliders.size(); ++i) {
            auto [a, ta, ca] = colliders[i];

            const glm::vec3 aMin = ta->position - ca->size * 0.5f;
            const glm::vec3 aMax = ta->position + ca->size * 0.5f;

            for (size_t j = i + 1; j < colliders.size(); ++j) {
                auto [b, tb, cb] = colliders[j];

                const glm::vec3 bMin = tb->position - cb->size * 0.5f;
                const glm::vec3 bMax = tb->position + cb->size * 0.5f;

                const bool intersects =
                    (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
                    (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
                    (aMin.z <= bMax.z && aMax.z >= bMin.z);

                if (intersects)
                    _eventBus->Emit<CollisionEvent>(a, b);
            }
        }
    }
};

}