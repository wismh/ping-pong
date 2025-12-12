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
#include "player_controll_system.h"
#include "utils.h"

namespace game {

class GameScene {
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<e::WindowSystem> _windowSystem;
    std::shared_ptr<e::EventBus> _eventBus;
    std::shared_ptr<e::Time> _time;

    std::shared_ptr<er::Camera> _camera;

    std::shared_ptr<eui::Layout> _prepareLayout;
    std::shared_ptr<eui::Label> _scoreLabel;
    GameState _gameState;
public:
    GameScene(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time,
        const std::shared_ptr<e::EventBus>& eventBus
    ) :
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _commandBuffer(commandBuffer),
        _windowSystem(windowSystem),
        _time(time),
        _eventBus(eventBus) {

    }

    std::shared_ptr<e::Node> Build() {
        _camera = er::Camera::CreateCamera();
        _camera->orthographic = true;
        _camera->nearClip = -1.f;
        _camera->aspect = _windowSystem->Size().x / static_cast<float>(_windowSystem->Size().y);

        auto node = std::make_shared<e::Node>();
        node->AddChild(BuildUINode());
        node->AddChild(BuildEcsNode());

        return node;
    }
private:
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

        return std::make_shared<e::NodeUI>(_commandBuffer, layout);
    }

    std::shared_ptr<e::Node> BuildEcsNode() {
        const auto nodeEcs = std::make_shared<e::NodeEcs>();

        auto& world = nodeEcs->GetWorld();

        world.AddSystem(std::make_unique<ecs::RenderSystem>(_commandBuffer, _camera));
        world.AddSystem(std::make_unique<ecs::PhysicsSystem>(_eventBus, _time));
        world.AddSystem(std::make_unique<BallSystem>(_eventBus, _windowSystem, _camera, _time, nodeEcs->GetWorld()));
        world.AddSystem(std::make_unique<PlayerControllerSystem>(_eventBus, _gameState));
        world.AddSystem(std::make_unique<GameUISystem>(_scoreLabel, _prepareLayout, _gameState));
        world.AddSystem(std::make_unique<GamePresenterSystem>(_eventBus, _windowSystem, _camera, world, _gameState));

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
        world.AttachComponent(entity, Player{});
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
        world.AttachComponent(entity, Player{});
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
};

}
