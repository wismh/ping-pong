#pragma once
#include "../engine/core/event_bus.h"
#include "../engine/ecs/ecs.h"
#include "../engine/ecs/rigidbody.h"

#include "input_actions.h"
#include "player.h"
#include "utils.h"

namespace game {

class PlayerControllerSystem final : public ecs::ISystem {
public:
    explicit PlayerControllerSystem(const std::shared_ptr<e::EventBus>& eventBus)
    {
        eventBus->Subscribe<e::InputEvent>([this](const e::InputEvent& e) {
            if (e.action == InputActions::PlayerMoveLeft)
                moveLeft = true;

            if (e.action == InputActions::PlayerMoveRight)
                moveRight = true;
        });
    }

    void Update(ecs::World & world) override {
        world.ForEachWith<Player, ecs::RigidBody>(
            [&](Player& p, ecs::RigidBody& rb)
        {
            if (moveLeft)  rb.velocity.y = +10.f;
            if (moveRight) rb.velocity.y = -10.f;
        });

        moveLeft = moveRight = false;
    }

private:
    bool moveLeft = false;
    bool moveRight = false;
};

}