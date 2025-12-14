#include "../../../engine/include/engine/ecs/physcis_system.h"
#include "../../../engine/include/engine/ecs/rigidbody.h"
#include "../../../engine/include/engine/ecs/transform.h"

#include "../../include/game/systems/game_presenter_system.h"

#include "../../include/game/components/ball.h"
#include "../../include/game/components/player.h"
#include "../../include/game/input_actions.h"

namespace game {

GamePresenterSystem::GamePresenterSystem(
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

void GamePresenterSystem::Update(ecs::World& world) {
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

void GamePresenterSystem::StartRound() {
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

void GamePresenterSystem::StopRound() {
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

}