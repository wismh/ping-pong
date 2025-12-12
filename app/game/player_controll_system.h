#pragma once
#include "../engine/core/event_bus.h"
#include "../engine/ecs/ecs.h"
#include "../engine/ecs/rigidbody.h"

#include "input_actions.h"
#include "player.h"
#include "utils.h"

namespace game {

class PlayerControllerSystem final : public ecs::ISystem {
    bool _moveLeft = false;
    bool _moveRight = false;
    GameState& _gameState;
public:
    explicit PlayerControllerSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        GameState& gameState
    ) : _gameState(gameState) {
        eventBus->Subscribe<e::InputEvent>([this](const e::InputEvent& e) {
            if (e.action == InputActions::PlayerMoveLeft)
                _moveLeft = true;

            if (e.action == InputActions::PlayerMoveRight)
                _moveRight = true;
        });
    }

    void Update(ecs::World & world) override {
        if (!_gameState.paused)
            world.ForEachWith<Player, ecs::RigidBody>([this](
                Player& player, ecs::RigidBody& rigidbody
            ){
                if (_moveLeft)
                    rigidbody.velocity.y = +15.f;
                if (_moveRight)
                    rigidbody.velocity.y = -15.f;
            });

        _moveLeft = _moveRight = false;
    }
};

}