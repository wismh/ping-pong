#pragma once
#include "game_scene.h"
#include "input_actions.h"
#include "utils.h"

namespace game {

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<e::Node> _rootNode;
    std::shared_ptr<e::InputSystem> _inputSystem;

    std::shared_ptr<GameScene> _gameScene;
public:
    Game(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time,
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::InputSystem>& inputSystem
    ) :
        _commandBuffer(commandBuffer),
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _inputSystem(inputSystem),
        _gameScene(std::make_shared<GameScene>(
            logger,
            commandBuffer,
            assetsDb,
            windowSystem,
            time,
            eventBus
        )) {

    }

    void OnStart() override {
        _inputSystem->AddBinding(SDL_SCANCODE_W, InputActions::PlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_A, InputActions::PlayerMoveLeft);
        _inputSystem->AddBinding(SDL_SCANCODE_S, InputActions::PlayerMoveRight);
        _inputSystem->AddBinding(SDL_SCANCODE_D, InputActions::PlayerMoveRight);

        _rootNode = std::make_shared<e::Node>();
        _rootNode->AddChild(_gameScene->Build());
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