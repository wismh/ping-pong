#pragma once
#include "engine/ecs/render_system.h"
#include "engine/ecs/renderable.h"
#include "engine/nodes/node_ui.h"
#include "engine/ui/label.h"
#include "engine/ui/layout.h"

#include "components/game_state.h"
#include "systems/player_controll_system.h"
#include "utils.h"

namespace game {

class GameScene {
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::WindowSystem> _windowSystem;
    std::shared_ptr<e::EventBus> _eventBus;
    std::shared_ptr<e::ApplicationState> _appState;
    std::shared_ptr<e::Time> _time;

    std::shared_ptr<er::Camera> _camera;

    std::shared_ptr<eui::Layout> _menuLayout;
    std::shared_ptr<eui::Layout> _prepareLayout;
    std::shared_ptr<eui::Label> _scoreLabel;
    std::shared_ptr<eui::Label> _resultLabel;
    GameState _gameState;
    Bounds _worldBounds {};
public:
    GameScene(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time,
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::ApplicationState>& appState
    );

    std::shared_ptr<e::Node> Build();
private:
    std::shared_ptr<eui::Layout> BuildUIMenu();
    std::shared_ptr<e::Node> BuildUINode();
    std::shared_ptr<e::Node> BuildEcsNode();

    void CreateRedPlayer(ecs::World& world);
    void CreateBluePlayer(ecs::World& world);
    void CreateBall(ecs::World& world);
    void CalcWorldBounds(
      const std::shared_ptr<e::WindowSystem>& windowSystem,
      const std::shared_ptr<er::Camera>& camera
    );
};

}
