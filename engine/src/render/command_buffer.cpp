#include "engine/render/command_buffer.h"

namespace engine::render {

CommandBuffer::CommandBuffer(
    const std::shared_ptr<IRenderBackend>& renderer
) :
    _renderer(renderer) {

}

void CommandBuffer::Execute() {
    _renderer->Execute(commands);
    commands.clear();
}

}