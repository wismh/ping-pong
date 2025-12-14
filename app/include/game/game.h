#pragma once
#include "utils.h"
#include "game_scene.h"

namespace game {

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<e::Node> _rootNode;
    std::shared_ptr<e::InputSystem> _inputSystem;
    std::shared_ptr<eui::UICanvas> _uiCanvas;
    std::shared_ptr<e::EventBus> _eventBus;

    std::shared_ptr<GameScene> _gameScene;
public:
    Game(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time,
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::InputSystem>& inputSystem,
        const std::shared_ptr<eui::UICanvas>& uiCanvas,
        const std::shared_ptr<e::ApplicationState>& appState
    );

    void OnStart() override;
    void OnUpdate() override;
    void OnDraw() override;
    void OnQuit() override;
};

}