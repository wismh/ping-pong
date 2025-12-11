#pragma once
#include "ball.h"
#include "utils.h"
#include "../engine/ecs/transform.h"

namespace game {

class BallSystem final : public ecs::ISystem {
    struct Bounds {
        float left, right, top, bottom;
    };

    Bounds _worldBounds;
    std::shared_ptr<e::Time> _time;
public:
    BallSystem(
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<er::Camera>& camera,
        const std::shared_ptr<e::Time>& time
    ) :
        _time(time)
    {
        auto topLeft = er::Camera::ScreenToWorld(
            {0, 0, 0},
            camera,
            windowSystem->Size()
        );
        auto bottomRight = er::Camera::ScreenToWorld(
            {windowSystem->Size(), 0},
            camera,
            windowSystem->Size()
        );

        _worldBounds = {
            topLeft.x, bottomRight.x, topLeft.y,bottomRight.y
        };
    }

    void Update(ecs::World& world) override {
        world.ForEachWith<Ball, ecs::Transform>([this](
            Ball& ball, ecs::Transform& transform
        ) {
            transform.position += ball.velocity * _time->deltaTime;

            if (transform.position.x > _worldBounds.right || transform.position.x < _worldBounds.left)
                ball.velocity.x = -ball.velocity.x;
            if (transform.position.y > _worldBounds.top || transform.position.y < _worldBounds.bottom)
                ball.velocity.y = -ball.velocity.y;
        });
    }
};

}