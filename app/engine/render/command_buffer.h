#pragma once
#include "commands.h"
#include "irender_backend.h"

namespace engine::render {

class CommandBuffer final {
    std::vector<Command> commands{};
    std::shared_ptr<IRenderBackend> _renderer;
public:
    explicit CommandBuffer(const std::shared_ptr<IRenderBackend>& renderer) :
        _renderer(renderer) {

    }

    template<typename T>
    void Push(const T& cmd) {
        commands.emplace_back(cmd);
    }

    void Execute() {
        _renderer->Execute(commands);
        commands.clear();
    }
};

}
