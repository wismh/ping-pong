#pragma once
#include "engine/ecs/ecs.h"
#include "engine/core/event_bus.h"
#include "engine/ecs/rigidbody.h"
#include "engine/ecs/transform.h"

#include "game/components/game_state.h"
#include "game/components/player.h"
#include "game/utils.h"

namespace game {

class PlayerControllerSystem final : public ecs::ISystem {
    bool _blueMoveLeft = false;
    bool _blueMoveRight = false;
    bool _redMoveLeft = false;
    bool _redMoveRight = false;

    float _reactionTimer;
    GameState& _gameState;
    Bounds& _worldBounds;
public:
    explicit PlayerControllerSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        GameState& gameState,
        Bounds& worldBounds
    );

    void Update(ecs::World& world) override;
private:
    void ControlByBlue(
        const Player& player,
        const ecs::Transform& transform,
        ecs::RigidBody& rigidbody
    );

    void ControlByRed(
        const Player& player,
        const ecs::Transform& transform,
        ecs::RigidBody& rigidbody
    );

    bool CanMove(
        const ecs::Transform& transform,
        const ecs::RigidBody& rigidbody
    ) const;
};

}