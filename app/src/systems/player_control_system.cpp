#include "../../include/game/input_actions.h"
#include "../../include/game/systems/player_controll_system.h"

namespace game {

PlayerControllerSystem::PlayerControllerSystem(
    const std::shared_ptr<e::EventBus>& eventBus,
    GameState& gameState,
    Bounds& worldBounds
) :
    _gameState(gameState),
    _worldBounds(worldBounds)
{
    eventBus->Subscribe<e::InputEvent>([this](const e::InputEvent& e) {
        if (e.action == InputActions::BluePlayerMoveLeft)
            _blueMoveLeft = true;

        if (e.action == InputActions::BluePlayerMoveRight)
            _blueMoveRight = true;

        if (e.action == InputActions::RedPlayerMoveLeft)
            _redMoveLeft = true;

        if (e.action == InputActions::RedPlayerMoveRight)
            _redMoveRight = true;
    });
}

void PlayerControllerSystem::Update(ecs::World& world) {
    if (!_gameState.waitForRound)
        world.ForEachWith<Player, ecs::Transform, ecs::RigidBody>([&](
            const Player& player, ecs::Transform& transform, ecs::RigidBody& rigidbody
        ){
            if (player.control == PlayerControlBy::BLUE_ACTIONS)
                ControlByBlue(player, transform, rigidbody);
            else if (player.control == PlayerControlBy::RED_ACTIONS)
                ControlByRed(player, transform, rigidbody);
        });

    _blueMoveLeft = _blueMoveRight = false;
    _redMoveLeft = _redMoveRight = false;
}

void PlayerControllerSystem::ControlByBlue(
    const Player& player,
    const ecs::Transform& transform,
    ecs::RigidBody& rigidbody
) {
    if (_blueMoveLeft)
        rigidbody.velocity.y = player.speed;
    if (_blueMoveRight)
        rigidbody.velocity.y = -player.speed;

    if (!CanMove(transform, rigidbody))
        rigidbody.velocity.y = 0;
}

void PlayerControllerSystem::ControlByRed(
    const Player& player,
    const ecs::Transform& transform,
    ecs::RigidBody& rigidbody
) {
    if (_redMoveLeft)
        rigidbody.velocity.y = player.speed;
    if (_redMoveRight)
        rigidbody.velocity.y = -player.speed;

    if (!CanMove(transform, rigidbody))
        rigidbody.velocity.y = 0;
}

bool PlayerControllerSystem::CanMove(
    const ecs::Transform& transform,
    const ecs::RigidBody& rigidbody
) const {
    if (rigidbody.velocity.y > 0.0f)
        return transform.position.y < _worldBounds.top;

    if (rigidbody.velocity.y < 0.0f)
        return transform.position.y > _worldBounds.bottom;

    return true;
}

}