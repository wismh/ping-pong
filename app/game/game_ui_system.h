#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ui/label.h"

#include "game_state.h"
#include "utils.h"

namespace game {

class GameUISystem final : public ecs::ISystem {
    std::shared_ptr<eui::Label> _scoreLabel;
    std::shared_ptr<eui::Layout> _prepareLayout;
    GameState& _gameState;
public:
    explicit GameUISystem(
        const std::shared_ptr<eui::Label>& scoreLabel,
        const std::shared_ptr<eui::Layout>& prepareLayout,
        GameState& gameState
    ):
        _scoreLabel(scoreLabel),
        _prepareLayout(prepareLayout),
        _gameState(gameState) {

    }

    void Update(ecs::World& world) override {
        _prepareLayout->visible = _gameState.paused;
        _scoreLabel->text = fmt::format(
            "{} : {}",
            _gameState.bluePlayerScore,
            _gameState.redPlayerScore
        );
    }
};

}