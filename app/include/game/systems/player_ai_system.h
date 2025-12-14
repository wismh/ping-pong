#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ecs/rigidbody.h"
#include "engine/ecs/transform.h"

#include "game/components/ai_params.h"
#include "game/components/game_state.h"
#include "game/components/player.h"
#include "game/utils.h"

namespace game {

class PlayerAISystem final : public ecs::ISystem {
    float _reactionTimer = 0.f;

    std::shared_ptr<e::Time> _time;
    GameState& _gameState;
    Bounds& _worldBounds;
    AiParams _aiParams;
public:
    explicit PlayerAISystem(
        const std::shared_ptr<e::Time>& time,
        GameState& gameState,
        Bounds& worldBounds,
        AiParams aiParams
    );

    void Update(ecs::World& world) override;
private:
    void ControlByAI(
        ecs::World& world,
        const Player& player, const ecs::Transform& transform,
        ecs::RigidBody& rigidbody
    );

    float PredictBallY(
        glm::vec2 ballPosition, glm::vec2 ballVelocity, float botX
    ) const;
};

}