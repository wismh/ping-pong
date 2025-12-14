#pragma once

#include "../components/game_state.h"
#include "../utils.h"

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
    );

    void Update(ecs::World& world) override;
private:
    void StartRound();
    void StopRound();
};

}