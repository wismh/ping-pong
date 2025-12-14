#pragma once

#include "engine/render/igpu_mesh.h"
#include "engine/resources/mesh_desc.h"
#include "engine/utils.h"

namespace engine::render {

class OpenGLMesh final : public IMesh {
    GLuint _vao;
    GLuint _vbo;
    size_t _verticesCount;
public:
    explicit OpenGLMesh(const MeshDesc& desc);
    void Draw() override;
};

}
