#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ui/label.h"
#include "engine/ui/layout.h"

#include "game/components/game_state.h"
#include "game/utils.h"

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
    );

    void Update(ecs::World& world) override;
};

}