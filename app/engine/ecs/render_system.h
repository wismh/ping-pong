#pragma once
#include "ecs.h"
#include "renderable.h"
#include "transform.h"
#include "engine/render/command_buffer.h"
#include "engine/render/camera.h"
#include "../utils.h"

namespace engine::ecs {

class RenderSystem final : public ecs::ISystem {
    std::shared_ptr<render::CommandBuffer> _commandBuffer;
    std::shared_ptr<render::Camera> _camera;
public:
    explicit RenderSystem(
        const std::shared_ptr<render::CommandBuffer>& commandBuffer,
        const std::shared_ptr<render::Camera>& camera
    ) :
        _commandBuffer(commandBuffer),
        _camera(camera) {

    }

    void Update(ecs::World& world) override {
        world.ForEachWith<Renderable, Transform>([this](
            const Renderable& renderable, const Transform& transform
        ) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, transform.position);
            model = glm::rotate(model, transform.rotation.x, {1,0,0});
            model = glm::rotate(model, transform.rotation.y, {0,1,0});
            model = glm::rotate(model, transform.rotation.z, {0,0,1});
            model = glm::scale(model, transform.scale);

            _commandBuffer->Push(render::CmdDrawMesh{
                renderable.texture,
                renderable.shader,
                renderable.mesh,
                model,
                _camera->GetView(),
                _camera->GetProjection()
            });
        });
    }
};

}