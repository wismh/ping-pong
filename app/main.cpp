#include "engine/core/engine.h"
#include "engine/render/camera.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"

namespace game {

namespace e = engine;
namespace er = engine::render;

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<er::IGraphicFabric> _graphicsFabric;
    std::shared_ptr<e::TexturePipe> _texturePipe;
    std::shared_ptr<e::ShaderPipe> _shaderPipe;
    std::shared_ptr<e::MeshPipe> _meshPipe;
    std::shared_ptr<spdlog::logger> _logger;

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
        const std::shared_ptr<e::TexturePipe>& texturePipe,
        const std::shared_ptr<e::ShaderPipe>& shaderPipe,
        const std::shared_ptr<e::MeshPipe>& meshPipe
    ) :
        _commandBuffer(commandBuffer),
        _graphicsFabric(graphicsFabric),
        _logger(logger->Get()),
        _texturePipe(texturePipe),
        _shaderPipe(shaderPipe),
        _meshPipe(meshPipe) {

    }

    void OnStart() override {
        _mesh = _meshPipe->Load("test", e::GetResourcePath("quad.mesh"));
        _shader = _shaderPipe->Load("test", e::GetResourcePath("default-shader.shader"));
        _texture = _texturePipe->Load("test", e::GetResourcePath("test.png"));

        _camera = er::Camera::CreateCamera();
        _camera->orthographic = true;
        _camera->nearClip = -1.f;
        _camera->aspect = 800.f / 600.f;

        vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        if (!vg) {
            _logger->error("Failed to init NanoVG");
            return;
        }

        auto path = e::GetResourcePath("Roboto_Condensed-Regular.ttf");
        font = nvgCreateFont(vg, "font", path.c_str());
        if (font < 0) {
             _logger->error("Failed to load font by path: {}", path);
            return;
        }
    }

    void OnUpdate(const float deltaTime) override {

    }

    void OnDraw(const float deltaTime) override {
        static glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(3, 3, 1));
        static glm::mat4 model2 = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));

        _commandBuffer->Push(er::CmdDrawMesh{
            _texture,
            _shader,
            _mesh,
            model,
            _camera->GetView(),
            _camera->GetProjection()
        });
        _commandBuffer->Push(er::CmdDrawMesh{
            _texture,
            _shader,
            _mesh,
            model2,
            _camera->GetView(),
            _camera->GetProjection()
        });

        model = glm::rotate(model, glm::radians(45.f * deltaTime), glm::vec3(0.f, 0.0f, 1.0f));

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

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
