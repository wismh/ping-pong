#pragma once
#include "ball.h"
#include "utils.h"
#include "../engine/ecs/rigidbody.h"
#include "../engine/ecs/transform.h"
#include "../engine/ecs/physcis_system.h"

namespace game {

class BallSystem final : public ecs::ISystem {
    struct Bounds {
        float left, right, top, bottom;
    };

    Bounds _worldBounds;
    std::shared_ptr<e::Time> _time;
    ecs::World& _world;
public:
    BallSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<er::Camera>& camera,
        const std::shared_ptr<e::Time>& time,
        ecs::World& world
    ) :
        _time(time),
        _world(world)
    {
        CalcWorldBounds(windowSystem, camera);
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
            if (transform.position.y > _worldBounds.top || transform.position.y < _worldBounds.bottom)
                rigidBody.velocity.y = -rigidBody.velocity.y;
        });
    }
private:
    void CalcWorldBounds(
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<er::Camera>& camera
    ) {
        const auto topLeft = er::Camera::ScreenToWorld(
            {0, 0, 0},
            camera,
            windowSystem->Size()
        );
        const auto bottomRight = er::Camera::ScreenToWorld(
            {windowSystem->Size(), 0},
            camera,
            windowSystem->Size()
        );

        _worldBounds = {
            topLeft.x, bottomRight.x, topLeft.y,bottomRight.y
        };
    }
};

}