#pragma once
#include "utils.h"
#include "game_scene.h"

namespace game {

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<e::Node> _rootNode;

    std::shared_ptr<GameScene> _gameScene;
public:
    Game(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time
    ) :
        _commandBuffer(commandBuffer),
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _gameScene(std::make_shared<GameScene>(
            logger,
            commandBuffer,
            assetsDb,
            windowSystem,
            time
        )) {

    }

    void OnStart() override {
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