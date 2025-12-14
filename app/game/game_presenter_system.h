#pragma once
#include "../engine/ecs/physcis_system.h"
#include "../engine/ecs/rigidbody.h"
#include "../engine/ecs/transform.h"
#include "ball.h"
#include "game_state.h"
#include "input_actions.h"
#include "player.h"
#include "utils.h"

namespace game {

class GamePresenterSystem final : public ecs::ISystem {
    Bounds& _worldBounds;
    ecs::World& _world;
    GameState& _gameState;
public:
    GamePresenterSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        ecs::World& world,
        GameState& gameState,
        Bounds& worldBounds
    ) :
        _worldBounds(worldBounds),
        _world(world),
        _gameState(gameState)
    {
        eventBus->Subscribe<e::InputEvent>([this](const e::InputEvent& e) {
          if (e.action == InputActions::StartRound)
              StartRound();
        });
    }

    void Update(ecs::World& world) override {
        world.ForEachWith<Ball, ecs::Transform>([this](
            Ball& ball, const ecs::Transform& transform
        ) {
            bool goal = false;
            constexpr float k_offset = 5;

            if (transform.position.x > _worldBounds.right + k_offset) {
                _gameState.bluePlayerScore += 1;
                _gameState.isBlueTurn = true;
                goal = true;
            } else if (transform.position.x < _worldBounds.left - k_offset) {
                _gameState.redPlayerScore += 1;
                _gameState.isBlueTurn = false;
                goal = true;
            }

            if (goal) {
                _gameState.waitForRound = true;
                StopRound();
            }
        });
    }
private:
    void StartRound() {
        if (!_gameState.waitForRound || _gameState.paused)
            return;

        _gameState.waitForRound = false;
        _world.ForEachWith<Ball, ecs::RigidBody, ecs::Transform>([this](
           Ball& ball, ecs::RigidBody& rigidBody, ecs::Transform& transform
        ) {
            rigidBody.velocity = {
                _gameState.isBlueTurn ? ball.speed : -ball.speed,
                -ball.speed,
                0
            };
        });
    }

    void StopRound() {
        if (_gameState.bluePlayerScore >= 5 || _gameState.redPlayerScore >= 5)
            _gameState.paused = true;

        _world.ForEachWith<Ball, ecs::RigidBody, ecs::Transform>([this](
           Ball& ball, ecs::RigidBody& rigidBody, ecs::Transform& transform
        ) {
            transform.position = {
                _gameState.isBlueTurn ? -10 : 10,
                0, 0
            };
            rigidBody.velocity = {};
        });
        _world.ForEachWith<Player, ecs::RigidBody, ecs::Transform>([this](
           Player& player, ecs::RigidBody& rigidBody, ecs::Transform& transform
        ) {
            transform.position.y = 0;
            rigidBody.velocity = {};
            player.reflectionCombo = 0;
        });
    }
};

}