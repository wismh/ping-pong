#pragma once
#include "ball.h"
#include "ball_system.h"
#include "../engine/ecs/render_system.h"
#include "utils.h"

namespace game {

class GameScene {
        std::shared_ptr<e::AssetsDb> _assetsDb;
        std::shared_ptr<spdlog::logger> _logger;
        std::shared_ptr<er::CommandBuffer> _commandBuffer;
        std::shared_ptr<e::WindowSystem> _windowSystem;
        std::shared_ptr<e::Time> _time;

        std::shared_ptr<er::Camera> _camera;
public:
    GameScene(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<e::AssetsDb>& assetsDb,
        const std::shared_ptr<e::WindowSystem>& windowSystem,
        const std::shared_ptr<e::Time>& time
    ) :
        _assetsDb(assetsDb),
        _logger(logger->Get()),
        _commandBuffer(commandBuffer),
        _windowSystem(windowSystem),
        _time(time){

    }

    std::shared_ptr<e::Node> Build() {
        _camera = er::Camera::CreateCamera();
        _camera->orthographic = true;
        _camera->nearClip = -1.f;
        _camera->aspect = _windowSystem->Size().x / static_cast<float>(_windowSystem->Size().y);

        const auto nodeEcs = std::make_shared<e::NodeEcs>();

        nodeEcs->GetWorld().AddSystem(std::make_unique<ecs::RenderSystem>(_commandBuffer, _camera));
        nodeEcs->GetWorld().AddSystem(std::make_unique<BallSystem>(_windowSystem, _camera, _time));

        for (int i = 0; i < 50; ++i) {
            const auto entity = nodeEcs->GetWorld().CreateEntity();

            nodeEcs->GetWorld().AttachComponent(entity, ecs::Renderable{
                .mesh = _assetsDb->Get<er::IMesh>("quad.mesh"),
                .shader = _assetsDb->Get<er::IShader>("default-shader.shader"),
                .texture = _assetsDb->Get<er::ITexture>("ball.png")
            });
            nodeEcs->GetWorld().AttachComponent(entity, ecs::Transform{
                .position = {
                    -15 + rand() % 30,
                    -15 + rand() % 30,
                     0
                },
                .rotation = {0, 0, 0},
                .scale = {3, 3, 1},
            });
            nodeEcs->GetWorld().AttachComponent(entity, Ball{
                .velocity = {
                    (-10 + rand() % 20),
                    (-10 + rand() % 20),
                    0.f
                }
            });
        }

        return nodeEcs;
    }
};

}
