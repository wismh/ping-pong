#pragma once
#include <memory>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace engine::render {

class CameraCache;

class Camera {
    friend class CameraCache;
private:
    CameraCache* _cache;

    explicit Camera(CameraCache* c):
        _cache(c) {}
public:

    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float pitch = 0.0f;
    float yaw = -90.0f;

    float fov = 60.0f;
    float aspect = 16.0f / 9.0f;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    bool orthographic = false;
    float orthoSize = 10.0f;

    const glm::mat4& GetView() const;
    const glm::mat4& GetProjection() const;

    static std::shared_ptr<Camera> CreateCamera();
    static glm::vec3 ScreenToWorld(
        const glm::vec3& screenPosition,
        const std::shared_ptr<Camera>& camera,
        const glm::ivec2& windowSize);
};

class CameraCache {
public:
    CameraCache():
        prevState(Camera(this)) {}

    const glm::mat4& GetView(const Camera& camera);
    const glm::mat4& GetProjection(const Camera& camera);

private:
    Camera prevState;
    glm::mat4 view{};
    glm::mat4 projection{};

    bool HasChanged(const Camera& camera) const;
    void Recalc(const Camera& camera);
};
}
