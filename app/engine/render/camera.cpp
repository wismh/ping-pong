#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace engine::render {

const glm::mat4& Camera::GetView() const {
    return _cache->GetView(*this);
}

const glm::mat4& Camera::GetProjection() const {
    return _cache->GetProjection(*this);
}

std::shared_ptr<Camera> Camera::CreateCamera() {
    return std::shared_ptr<Camera>(new Camera(
        new CameraCache()
    ));
}

const glm::mat4& CameraCache::GetView(const Camera& camera) {
    if (HasChanged(camera))
        Recalc(camera);
    return view;
}

const glm::mat4& CameraCache::GetProjection(const Camera& camera) {
    if (HasChanged(camera))
        Recalc(camera);
    return projection;
}

bool CameraCache::HasChanged(const Camera& camera) const {
    return memcmp(&camera, &prevState, sizeof(Camera)) != 0;
}

void CameraCache::Recalc(const Camera& camera) {
    prevState = camera;

    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front = glm::normalize(front);

    view = glm::lookAt(camera.position, camera.position + front, {0,1,0});

    if (camera.orthographic) {
        const float h = camera.orthoSize;
        const float w = camera.orthoSize * camera.aspect;
        projection = glm::ortho(
            -w, w, -h, h,
            camera.nearClip, camera.farClip
        );
    } else {
        projection = glm::perspective(
            glm::radians(camera.fov),
            camera.aspect,
            camera.nearClip,
            camera.farClip
        );
    }
}

}
