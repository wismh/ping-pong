#pragma once
#include "engine/render/command_buffer.h"
#include "engine/render/camera.h"
#include "engine/utils.h"

#include "ecs.h"

namespace engine::ecs {

class RenderSystem final : public ecs::ISystem {
    std::shared_ptr<render::CommandBuffer> _commandBuffer;
    std::shared_ptr<render::Camera> _camera;
public:
    explicit RenderSystem(
        const std::shared_ptr<render::CommandBuffer>& commandBuffer,
        const std::shared_ptr<render::Camera>& camera
    );

    void Update(ecs::World& world) override;
};

}