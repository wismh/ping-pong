#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ecs/rigidbody.h"
#include "game_state.h"

#include "player.h"
#include "utils.h"

namespace game {

class PlayerAISystem final : public ecs::ISystem {
    float _reactionTimer = 0.f;

    std::shared_ptr<e::Time> _time;
    GameState& _gameState;
    Bounds& _worldBounds;
public:
    explicit PlayerAISystem(
        const std::shared_ptr<e::Time>& time,
        GameState& gameState,
        Bounds& worldBounds
    ) :
        _time(time),
        _gameState(gameState),
        _worldBounds(worldBounds)
    {
    }

    void Update(ecs::World& world) override {
        if (!_gameState.paused)
            world.ForEachWith<Player, ecs::Transform, ecs::RigidBody>([&](
                const Player& player, ecs::Transform& transform, ecs::RigidBody& rigidbody
            ){
                if (player.control == PlayerControlBy::AI)
                    ControlByAI(world, player, transform, rigidbody);
            });
    }
private:

    void ControlByAI(ecs::World& world, const Player& player, const ecs::Transform& transform, ecs::RigidBody& rigidbody) {
        _reactionTimer -= _time->deltaTime;

        if (_reactionTimer > 0.0f)
            return;

        _reactionTimer = player.reactionTime;

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
               diff * player.difficulty,
               -player.speed,
               player.speed
            );

            rigidbody.velocity.y = targetSpeed;
        });
    }

    float PredictBallY(
        glm::vec2 ballPosition,
        glm::vec2 ballVelocity,
        float botX
    ) {
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
};

}