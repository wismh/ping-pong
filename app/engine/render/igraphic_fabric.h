#pragma once

#include "commands.h"

namespace engine::render {

struct VertexBufferDesc {
    size_t size;
    const void* data;
};

struct PipelineDesc {
    const char* vertexShaderSrc;
    const char* fragmentShaderSrc;
};

class IGraphicFabric {
public:
    virtual ~IGraphicFabric() = default;

    virtual BufferHandle CreateVertexBuffer(const VertexBufferDesc& desc) = 0;
    virtual PipelineHandle CreatePipeline(const PipelineDesc& desc) = 0;
};

}