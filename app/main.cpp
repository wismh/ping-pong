#include "engine/core/engine.h"

namespace game {

namespace e = engine;
namespace er = engine::render;

class Game final : public e::IGame {
    std::shared_ptr<er::CommandBuffer> _commandBuffer;
    std::shared_ptr<er::IGraphicFabric> _graphicsFabric;
    std::shared_ptr<spdlog::logger> _logger;

    std::shared_ptr<er::IMesh> _mesh;
    std::shared_ptr<er::IShader> _shader;

    std::vector<float> triangleVertices{
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,

        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };
public:
    Game(
        const std::shared_ptr<e::Logger>& logger,
        const std::shared_ptr<er::CommandBuffer>& commandBuffer,
        const std::shared_ptr<er::IGraphicFabric>& graphicsFabric
    ) :
        _commandBuffer(commandBuffer),
        _graphicsFabric(graphicsFabric),
        _logger(logger->Get()) {

    }

    void OnStart() override {
        const char* vertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

        const char* fragmentSrc = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

        std::string errorMessage;

        if (!_graphicsFabric->TryCreateShader({
            .vertexShaderSrc = vertexSrc,
            .fragmentShaderSrc = fragmentSrc,
        }, _shader, errorMessage)) {
            _logger->error(errorMessage);
            return;
        }

        if (!_graphicsFabric->TryCreateMesh({
            triangleVertices, {}
        }, _mesh, errorMessage)) {
            _logger->error(errorMessage);
            return;
        }
    }

    void OnUpdate() override {

    }

    void OnDraw() override {
        _commandBuffer->Push(er::CmdUseShader{_shader});
        _commandBuffer->Push(er::CmdDrawMesh{
            _mesh, glm::mat4(1.0f),
        });
    }

    void OnQuit() override {

    }
};

}

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
