#include "engine/ecs/render_system.h"

#include "engine/ecs/transform.h"
#include "engine/ecs/renderable.h"

namespace engine::ecs {

RenderSystem::RenderSystem(
       const std::shared_ptr<render::CommandBuffer>& commandBuffer,
       const std::shared_ptr<render::Camera>& camera
   ) :
       _commandBuffer(commandBuffer),
       _camera(camera) {

}

void RenderSystem::Update(ecs::World& world) {
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

}