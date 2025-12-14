#pragma once
#include "game_scene.h"
#include "input_actions.h"
#include "utils.h"
#include "engine/nodes/node_ui.h"
#include "engine/ui/label.h"
#include "engine/ui/layout.h"
#include "engine/audio/audio_events_manager.h"

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
    ) :
        _commandBuffer(commandBuffer),
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _eventBus(eventBus),
        _inputSystem(inputSystem),
        _uiCanvas(uiCanvas),
        _gameScene(std::make_shared<GameScene>(
            logger,
            commandBuffer,
            assetsDb,
            windowSystem,
            time,
            eventBus,
            appState
        )) {

    }

    void OnStart() override {
        _inputSystem->AddBinding(SDL_SCANCODE_W, InputActions::BluePlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_A, InputActions::BluePlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_S, InputActions::BluePlayerMoveRight);
        _inputSystem->AddBinding(SDL_SCANCODE_D, InputActions::BluePlayerMoveRight);
        _inputSystem->AddBinding(SDL_SCANCODE_UP, InputActions::RedPlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_LEFT, InputActions::RedPlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_DOWN, InputActions::RedPlayerMoveRight);
        _inputSystem->AddBinding(SDL_SCANCODE_RIGHT, InputActions::RedPlayerMoveRight);
        _inputSystem->AddBinding(SDL_SCANCODE_SPACE, InputActions::StartRound);

        auto path = e::AssetsDb::GetAssetPath("Roboto_Condensed-Regular.ttf");
        auto font = nvgCreateFont(_uiCanvas->GetContext(), "font", path.c_str());
        if (font < 0) {
            _logger->error("Failed to load font by path: {}", path);
            return;
        }

        _rootNode = std::make_shared<e::Node>();
        _rootNode->AddChild(_gameScene->Build());

        _eventBus->Emit<e::PlayMusicEvent>("music.wav");
    }

    void OnUpdate() override {
        _rootNode->Update();
    }

    void OnDraw() override {

    }

    void OnQuit() override {
    }
};

}