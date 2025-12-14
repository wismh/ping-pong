#include "../../include/game/systems/player_ai_system.h"

#include "../../include/game/components/ball.h"

namespace game {

PlayerAISystem::PlayerAISystem(
    const std::shared_ptr<e::Time>& time,
    GameState& gameState,
    Bounds& worldBounds,
    AiParams aiParams
) :
    _time(time),
    _gameState(gameState),
    _worldBounds(worldBounds),
    _aiParams(aiParams)
{
}

void PlayerAISystem::Update(ecs::World& world) {
    if (!_gameState.waitForRound)
        world.ForEachWith<Player, ecs::Transform, ecs::RigidBody>([&](
            const Player& player, ecs::Transform& transform, ecs::RigidBody& rigidbody
        ){
            if (player.control == PlayerControlBy::AI)
                ControlByAI(world, player, transform, rigidbody);
        });
}

void PlayerAISystem::ControlByAI(
    ecs::World& world,
    const Player& player,
    const ecs::Transform& transform,
    ecs::RigidBody& rigidbody
) {
    _reactionTimer -= _time->deltaTime;

    if (_reactionTimer > 0.0f)
        return;

    _reactionTimer =
            _aiParams.baseReactionTime
                + _aiParams.increaseReactionTimeByReflection * player.reflectionCombo
                - _aiParams.decreaseReactionTimeByScore * _gameState.bluePlayerScore;

    world.ForEachWith<Ball, ecs::RigidBody, ecs::Transform>([&](
        const Ball& ball, const ecs::RigidBody& ballRigidbody, const ecs::Transform& ballTransform
    ) {
        const float ballY = PredictBallY(
            ballTransform.position,
            ballRigidbody.velocity,
            transform.position.x
        );
        const float diff = ballY - transform.position.y;
        const float targetSpeed = std::clamp(
           diff * _aiParams.difficulty,
           -player.speed,
           player.speed
        );

        rigidbody.velocity.y = targetSpeed;
    });
}

float PlayerAISystem::PredictBallY(
    const glm::vec2 ballPosition,
    const glm::vec2 ballVelocity,
    const float botX
) const {
    if (ballVelocity.x <= 0.0f)
        return ballPosition.y;

    const float time = (botX - ballPosition.x) / ballVelocity.x;
    const float predictedY = ballPosition.y + ballVelocity.y * time;

    const float height = _worldBounds.top - _worldBounds.bottom;
    const float period = height * 2.0f;

    float mod = fmod(predictedY - _worldBounds.bottom, period);
    if (mod < 0.0f)
        mod += period;

    if (mod > height)
        return _worldBounds.top - (mod - height);
    return _worldBounds.bottom + mod;
}

}