#pragma once
#include "../engine/ecs/physcis_system.h"
#include "../engine/ecs/rigidbody.h"
#include "../engine/ecs/transform.h"
#include "ball.h"
#include "game_state.h"
#include "utils.h"

namespace game {

class BallSystem final : public ecs::ISystem {
    std::shared_ptr<e::Time> _time;
    Bounds& _worldBounds;
    ecs::World& _world;
public:
    BallSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::Time>& time,
        ecs::World& world,
        Bounds& worldBounds
    ) :
        _time(time),
        _worldBounds(worldBounds),
        _world(world)
    {
        eventBus->Subscribe<ecs::CollisionEvent>([&](const ecs::CollisionEvent& e) {
            OnCollision(e);
        });
    }

    void OnCollision(const ecs::CollisionEvent& e) {
        if (_world.HasComponent<Ball>(e.a) && _world.HasComponent<ecs::RigidBody>(e.a)) {
            auto& rd = _world.GetComponent<ecs::RigidBody>(e.a);
            rd.velocity.x = -rd.velocity.x;
        }
    }

    void Update(ecs::World& world) override {
        world.ForEachWith<Ball, ecs::RigidBody, ecs::Transform>([this](
            Ball& ball, ecs::RigidBody& rigidBody, const ecs::Transform& transform
        ) {
            if (transform.position.y > _worldBounds.top)
                rigidBody.velocity.y = -ball.speed;
            if (transform.position.y < _worldBounds.bottom)
                rigidBody.velocity.y = ball.speed;
        });
    }
};

}