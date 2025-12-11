#pragma once
#include "../engine/ecs/physcis_system.h"
#include "../engine/ecs/render_system.h"
#include "../engine/ecs/rigidbody.h"
#include "ball.h"
#include "ball_system.h"
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

        const auto nodeEcs = std::make_shared<e::NodeEcs>();

        nodeEcs->GetWorld().AddSystem(std::make_unique<ecs::RenderSystem>(_commandBuffer, _camera));
        nodeEcs->GetWorld().AddSystem(std::make_unique<ecs::PhysicsSystem>(_eventBus, _time));
        nodeEcs->GetWorld().AddSystem(std::make_unique<BallSystem>(_eventBus, _windowSystem, _camera, _time, nodeEcs->GetWorld()));
        nodeEcs->GetWorld().AddSystem(std::make_unique<PlayerControllerSystem>(_eventBus));

        CreateBall(nodeEcs->GetWorld());
        CreateBluePlayer(nodeEcs->GetWorld());
        CreateRedPlayer(nodeEcs->GetWorld());

        return nodeEcs;
    }
private:
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
            .position = {0, 0, 0},
            .rotation = {0, 0, 0},
            .scale = {3, 3, 1},
        });
        world.AttachComponent(entity, ecs::RigidBody{
            .velocity = {
                -15,
                -15,
                0.f
            }
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
