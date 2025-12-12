#pragma once
#include "game_scene.h"
#include "input_actions.h"
#include "utils.h"
#include "engine/nodes/node_ui.h"
#include "engine/ui/label.h"
#include "engine/ui/layout.h"

namespace game {

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<e::Node> _rootNode;
    std::shared_ptr<e::InputSystem> _inputSystem;
    std::shared_ptr<eui::UICanvas> _uiCanvas;

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
        const std::shared_ptr<eui::UICanvas>& uiCanvas
    ) :
        _commandBuffer(commandBuffer),
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _inputSystem(inputSystem),
        _uiCanvas(uiCanvas),
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

        auto path = e::AssetsDb::GetAssetPath("Roboto_Condensed-Regular.ttf");
        auto font = nvgCreateFont(_uiCanvas->GetContext(), "font", path.c_str());
        if (font < 0) {
            _logger->error("Failed to load font by path: {}", path);
            return;
        }

        _rootNode = std::make_shared<e::Node>();
        _rootNode->AddChild(_gameScene->Build());
        _rootNode->AddChild(BuildUI());
    }

    void OnUpdate() override {
        _rootNode->Update();
    }

    void OnDraw() override {

    }

    void OnQuit() override {
    }
private:
    std::shared_ptr<e::Node> BuildUI() {
        const auto label = std::make_shared<eui::Label>();
        label->text = "0 | 0";
        label->color = {255, 255, 255, 255};
        label->fontName = "font";
        label->fontSize = 32;

        const auto layout = std::make_shared<eui::Layout>();
        layout->align = eui::Align::Center;
        layout->direction = eui::Direction::Horizontal;
        layout->size = {800, 600};

        layout->children.emplace_back(label);

        return std::make_shared<e::NodeUI>(_commandBuffer, layout);
    }
};

}