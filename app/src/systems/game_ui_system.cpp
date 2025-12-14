#include "../../../engine/include/engine/ui/layout.h"

#include "../../include/game/systems/game_ui_system.h"

namespace game {

GameUISystem::GameUISystem(
    const std::shared_ptr<eui::Label>& scoreLabel,
    const std::shared_ptr<eui::Layout>& prepareLayout,
    const std::shared_ptr<eui::Layout>& menuLayout,
    const std::shared_ptr<eui::Label>& resultLabel,
    GameState& gameState
):
    _scoreLabel(scoreLabel),
    _resultLabel(resultLabel),
    _prepareLayout(prepareLayout),
    _menuLayout(menuLayout),
    _gameState(gameState) {

}

void GameUISystem::Update(ecs::World& world) {
    _prepareLayout->visible = _gameState.waitForRound;
    _scoreLabel->text = fmt::format(
        "{} : {}",
        _gameState.bluePlayerScore,
        _gameState.redPlayerScore
    );

    if (_gameState.paused) {
        _menuLayout->visible = true;
        _resultLabel->text =
            _gameState.bluePlayerScore > _gameState.redPlayerScore ?
                "You won!" : "You lost!";
    }
}

}