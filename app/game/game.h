#pragma once
#include "render_system.h"
#include "utils.h"

namespace game {

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<er::IGraphicFabric> _graphicsFabric;
    std::shared_ptr<e::AssetsDb> _assetsDb;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<e::Node> _rootNode;

    std::shared_ptr<er::IMesh> _mesh;
    std::shared_ptr<er::IShader> _shader;
    std::shared_ptr<er::ITexture> _texture;
    std::shared_ptr<er::Camera> _camera;

    NVGcontext* vg;
    int font;
public:
    Game(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<er::IGraphicFabric>& graphicsFabric,
        const std::shared_ptr<e::AssetsDb>& assetsDb
    ) :
        _commandBuffer(commandBuffer),
        _graphicsFabric(graphicsFabric),
        _logger(logger->Get()),
        _assetsDb(assetsDb) {

    }

    void OnStart() override {
        _mesh = _assetsDb->Load<er::IMesh>("quad.mesh");
        _shader = _assetsDb->Load<er::IShader>("default-shader.shader");
        _texture = _assetsDb->Load<er::ITexture>("test.png");

        _camera = er::Camera::CreateCamera();
        _camera->orthographic = true;
        _camera->nearClip = -1.f;
        _camera->aspect = 800.f / 600.f;

        _rootNode = std::make_shared<e::Node>(nullptr);

        const auto nodeEcs = std::make_shared<e::NodeEcs>(_rootNode);

        nodeEcs->GetWorld().AddSystem(
            std::make_unique<RenderSystem>(_commandBuffer, _camera)
        );

        for (int i = 0; i < 50; ++i) {
            auto entity = nodeEcs->GetWorld().CreateEntity();

            nodeEcs->GetWorld().AttachComponent(entity, Renderable{
                .mesh = _mesh,
                .shader = _shader,
                .texture = _texture
            });
            nodeEcs->GetWorld().AttachComponent(entity, Transform{
                .position = {
                    -15 + rand() % 30,
                    -15 + rand() % 30,
                     0
                },
                .rotation = {0, 0, 0},
                .scale = {3, 3, 1},
            });
        }

        _rootNode->AddChild(nodeEcs);


        vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        if (!vg) {
            _logger->error("Failed to init NanoVG");
            return;
        }

        auto path = e::AssetsDb::GetAssetPath("Roboto_Condensed-Regular.ttf");
        font = nvgCreateFont(vg, "font", path.c_str());
        if (font < 0) {
             _logger->error("Failed to load font by path: {}", path);
            return;
        }
    }

    void OnUpdate(const float deltaTime) override {
        _rootNode->Update();
    }

    void OnDraw(const float deltaTime) override {
        _commandBuffer->Push(er::CmdCustomDraw{
            [this]() {
                const float w = 800;
                const float h = 600;

                nvgBeginFrame(vg, w, h, w/h);

                nvgFontSize(vg, 32.0f);
                nvgFontFace(vg, "font");
                nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                nvgText(vg, w/2 - 120, 100, "Hello NanoVG!", nullptr);

                nvgEndFrame(vg);
            }
        });
    }

    void OnQuit() override {
        nvgDeleteGL3(vg);
    }
};

}