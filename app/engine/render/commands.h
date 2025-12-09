#pragma once
#include "../utils.h"

namespace engine::render {

using PipelineHandle = uint32_t;
using BufferHandle   = uint32_t;
using TextureHandle  = uint32_t;

struct CmdSetPipeline {
    PipelineHandle pipeline;
};

struct CmdBindVertexBuffer {
    BufferHandle buffer;
};

struct CmdBindTexture {
    TextureHandle texture;
    uint32_t slot;
};

struct CmdDraw {
    uint32_t vertexCount;
    uint32_t firstVertex;
};

using Command = std::variant<
    CmdSetPipeline,
    CmdBindVertexBuffer,
    CmdBindTexture,
    CmdDraw
>;

}