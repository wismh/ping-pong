#pragma once
#include "engine/ecs/physcis_system.h"
#include "engine/ecs/render_system.h"
#include "engine/ecs/rigidbody.h"
#include "engine/nodes/node_ui.h"
#include "engine/ui/label.h"
#include "engine/ui/layout.h"

#include "ball.h"
#include "ball_system.h"
#include "game_presenter_system.h"
#include "game_state.h"
#include "game_ui_system.h"
#include "player_ai_system.h"
#include "player_controll_system.h"
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
    Bounds _worldBounds;
public:
    GameScene(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time,
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::ApplicationState>& appState
    ) :
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _commandBuffer(commandBuffer),
        _windowSystem(windowSystem),
        _time(time),
        _eventBus(eventBus),
        _appState(appState) {

    }

    std::shared_ptr<e::Node> Build() {
        _camera = er::Camera::CreateCamera();
        _camera->orthographic = true;
        _camera->nearClip = -1.f;
        _camera->aspect = _windowSystem->Size().x / static_cast<float>(_windowSystem->Size().y);

        CalcWorldBounds(_windowSystem, _camera);

        auto node = std::make_shared<e::Node>();
        auto uiNode = BuildUINode();
        auto ecsNode = BuildEcsNode();

        node->AddChild(ecsNode);
        node->AddChild(uiNode);

        return node;
    }
private:

    std::shared_ptr<eui::Layout> BuildUIMenu() {
        _menuLayout = std::make_shared<eui::Layout>();
        _menuLayout->relative = false;
        _menuLayout->size = {800, 600};
        _menuLayout->align = eui::Align::Center;
        _menuLayout->crossAlign = eui::Align::Center;
        _menuLayout->color = {0, 0, 0, 150};

        const auto layout = std::make_shared<eui::Layout>();
        layout->size = {400, 300};
        layout->color = {20, 50, 130, 255};
        layout->direction = eui::Direction::Vertical;
        layout->gap = {0, 20};
        layout->radius = 50;
        layout->strokeWidth = 2;
        layout->strokeColor = {130, 180, 230, 255};

        const auto header = std::make_shared<eui::Layout>();
        header->size = {400, 80};
        header->align = eui::Align::Center;
        header->crossAlign = eui::Align::End;

        _resultLabel = std::make_shared<eui::Label>();
        _resultLabel->text = "Ping-Pong!";
        _resultLabel->color = {255, 255, 255, 255};
        _resultLabel->fontName = "font";
        _resultLabel->fontSize = 32;

        header->children.emplace_back(_resultLabel);
        layout->children.emplace_back(header);

        const auto layoutButtons = std::make_shared<eui::Layout>();
        layoutButtons->size = {400, 200};
        layoutButtons->align = eui::Align::Center;
        layoutButtons->crossAlign = eui::Align::Center;
        layoutButtons->gap = {50, 0};

        const auto restartButton = std::make_shared<eui::Layout>();
        restartButton->size = {100, 100};
        restartButton->color = {50, 70, 180, 255};
        restartButton->image = _assetsDb->Get<e::UIImage>("play.png");
        restartButton->clickable = true;
        restartButton->onClick = [this]() {
            _gameState.bluePlayerScore = 0;
            _gameState.redPlayerScore = 0;
            _menuLayout->visible = false;
        };

        const auto exitButton = std::make_shared<eui::Layout>();
        exitButton->size = {100, 100};
        exitButton->color = {50, 70, 180, 255};
        exitButton->image = _assetsDb->Get<e::UIImage>("door.png");
        exitButton->clickable = true;
        exitButton->onClick = [this]() {
            _appState->Quit();
        };

        layoutButtons->children.emplace_back(restartButton);
        layoutButtons->children.emplace_back(exitButton);

        layout->children.emplace_back(layoutButtons);

        _menuLayout->children.emplace_back(layout);

        return _menuLayout;
    }

    std::shared_ptr<e::Node> BuildUINode() {
        const auto label = std::make_shared<eui::Label>();
        label->text = "Press 'space' for start round!";
        label->color = {255, 255, 255, 255};
        label->fontName = "font";
        label->fontSize = 32;

        _prepareLayout = std::make_shared<eui::Layout>();
        _prepareLayout->align = eui::Align::Center;
        _prepareLayout->crossAlign = eui::Align::Center;
        _prepareLayout->direction = eui::Direction::Horizontal;
        _prepareLayout->size = {800, 400};

        _prepareLayout->children.emplace_back(label);

        _scoreLabel = std::make_shared<eui::Label>();
        _scoreLabel->text = "0 | 0";
        _scoreLabel->color = {255, 255, 255, 255};
        _scoreLabel->fontName = "font";
        _scoreLabel->fontSize = 32;

        const auto layout = std::make_shared<eui::Layout>();
        layout->align = eui::Align::Start;
        layout->crossAlign = eui::Align::Center;
        layout->direction = eui::Direction::Vertical;
        layout->size = {800, 600};

        layout->children.emplace_back(_scoreLabel);
        layout->children.emplace_back(_prepareLayout);
        layout->children.emplace_back(BuildUIMenu());

        return std::make_shared<e::NodeUI>(_eventBus, _commandBuffer, layout);
    }

    std::shared_ptr<e::Node> BuildEcsNode() {
        const auto nodeEcs = std::make_shared<e::NodeEcs>();

        auto& world = nodeEcs->GetWorld();

        world.AddSystem(std::make_unique<ecs::RenderSystem>(_commandBuffer, _camera));
        world.AddSystem(std::make_unique<ecs::PhysicsSystem>(_eventBus, _time));
        world.AddSystem(std::make_unique<BallSystem>(_eventBus, _time, world, _worldBounds));
        world.AddSystem(std::make_unique<PlayerControllerSystem>(_eventBus, _gameState, _worldBounds));
        world.AddSystem(std::make_unique<GameUISystem>(_scoreLabel, _prepareLayout, _menuLayout, _resultLabel, _gameState));
        world.AddSystem(std::make_unique<GamePresenterSystem>(_eventBus, world, _gameState, _worldBounds));
        world.AddSystem(std::make_unique<PlayerAISystem>(_time, _gameState, _worldBounds));

        CreateBall(world);
        CreateBluePlayer(world);
        CreateRedPlayer(world);

        return nodeEcs;
    }

    void CreateRedPlayer(ecs::World& world) {
        const auto entity = world.CreateEntity();

        world.AttachComponent(entity, ecs::Renderable{
            .mesh = _assetsDb->Get<er::IMesh>("quad.mesh"),
            .shader = _assetsDb->Get<er::IShader>("default-shader.shader"),
            .texture = _assetsDb->Get<er::ITexture>("red-player.png")
        });
        world.AttachComponent(entity, ecs::Transform{
            .position = {
                12.f,
                0.f,
                0.f
            },
            .rotation = {0, 0, 0},
            .scale = {1.f, 5, 1},
        });
        world.AttachComponent(entity, ecs::BoxCollider{
           .size = {
               0.5f,
               4.f,
               1
           }
       });
        world.AttachComponent(entity, ecs::RigidBody{});
        world.AttachComponent(entity, Player{
            .control = PlayerControlBy::AI
        });
    }

    void CreateBluePlayer(ecs::World& world) {
        const auto entity = world.CreateEntity();

        world.AttachComponent(entity, ecs::Renderable{
            .mesh = _assetsDb->Get<er::IMesh>("quad.mesh"),
            .shader = _assetsDb->Get<er::IShader>("default-shader.shader"),
            .texture = _assetsDb->Get<er::ITexture>("blue-player.png")
        });
        world.AttachComponent(entity, ecs::Transform{
            .position = {
                -12.f,
                0.f,
                0.f
            },
            .rotation = {0, 0, 0},
            .scale = {1.f, 5, 1},
        });
        world.AttachComponent(entity, ecs::BoxCollider{
            .size = {
                0.5f,
                4.f,
                1
            }
        });
        world.AttachComponent(entity, ecs::RigidBody{});
        world.AttachComponent(entity, Player{
            .control = PlayerControlBy::BLUE_ACTIONS
        });
    }

    void CreateBall(ecs::World& world) {
        const auto entity = world.CreateEntity();

        world.AttachComponent(entity, ecs::Renderable{
            .mesh = _assetsDb->Get<er::IMesh>("quad.mesh"),
            .shader = _assetsDb->Get<er::IShader>("default-shader.shader"),
            .texture = _assetsDb->Get<er::ITexture>("ball.png")
        });
        world.AttachComponent(entity, ecs::Transform{
            .position = {-10, 0, 0},
            .rotation = {0, 0, 0},
            .scale = {3, 3, 1},
        });
        world.AttachComponent(entity, ecs::RigidBody{
            .velocity = {}
        });
        world.AttachComponent(entity, ecs::BoxCollider{
            .size = {
                0.5f,
                0.5f,
                1
            }
        });
        world.AttachComponent(entity, Ball{});
    }

    void CalcWorldBounds(
      const std::shared_ptr<e::WindowSystem>& windowSystem,
      const std::shared_ptr<er::Camera>& camera
  ) {
        const auto topLeft = er::Camera::ScreenToWorld(
            {0, 0, 0},
            camera,
            windowSystem->Size()
        );

        const auto bottomRight = er::Camera::ScreenToWorld(
            {windowSystem->Size(), 0},
            camera,
            windowSystem->Size()
        );

        _worldBounds = {
            topLeft.x, bottomRight.x, topLeft.y,bottomRight.y
        };
    }
};

}
