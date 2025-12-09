#pragma once

#include "engine/render/igraphic_fabric.h"

namespace engine::render {
    class OpenGLFabric final : public IGraphicFabric {
    public:
        BufferHandle CreateVertexBuffer(const VertexBufferDesc& desc) override {
            return 0;
        }

        PipelineHandle CreatePipeline(const PipelineDesc& desc) override {
            return 0;
        }
    };

}