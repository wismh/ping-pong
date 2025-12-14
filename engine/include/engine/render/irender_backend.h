#pragma once
#include "commands.h"

namespace engine::render {

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;
    virtual void Execute(const std::vector<Command>& commands) = 0;
};

}