#pragma once
#include "commands.h"
#include "irender_backend.h"

namespace engine::render {

class CommandBuffer final {
    std::vector<Command> commands{};
    std::shared_ptr<IRenderBackend> _renderer;
public:
    explicit CommandBuffer(const std::shared_ptr<IRenderBackend>& renderer);
    void Execute();

    template<typename T>
    void Push(const T& cmd) {
        commands.emplace_back(cmd);
    }
};

}
