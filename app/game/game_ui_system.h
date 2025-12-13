#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ui/label.h"

#include "game_state.h"
#include "utils.h"

namespace game {

class GameUISystem final : public ecs::ISystem {
    std::shared_ptr<eui::Label> _scoreLabel;
    std::shared_ptr<eui::Label> _resultLabel;
    std::shared_ptr<eui::Layout> _prepareLayout;
    std::shared_ptr<eui::Layout> _menuLayout;
    GameState& _gameState;
public:
    explicit GameUISystem(
        const std::shared_ptr<eui::Label>& scoreLabel,
        const std::shared_ptr<eui::Layout>& prepareLayout,
        const std::shared_ptr<eui::Layout>& menuLayout,
        const std::shared_ptr<eui::Label>& resultLabel,
        GameState& gameState
    ):
        _scoreLabel(scoreLabel),
        _prepareLayout(prepareLayout),
        _menuLayout(menuLayout),
        _resultLabel(resultLabel),
        _gameState(gameState) {

    }

    void Update(ecs::World& world) override {
        _prepareLayout->visible = _gameState.paused;
        _scoreLabel->text = fmt::format(
            "{} : {}",
            _gameState.bluePlayerScore,
            _gameState.redPlayerScore
        );

        if (_gameState.bluePlayerScore + _gameState.redPlayerScore >= 3) {
            _gameState.paused = true;
            _menuLayout->visible = true;
            _resultLabel->text =
                _gameState.bluePlayerScore > _gameState.redPlayerScore ?
                    "You won!" : "You lost!";
        }
    }
};

}