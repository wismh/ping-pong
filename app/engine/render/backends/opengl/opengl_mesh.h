#pragma once
#include <memory>

#include "../../igpu_mesh.h"
#include "../../../resources/mesh_desc.h"
#include "../../../utils.h"

namespace engine::render {

class OpenGLMesh final : public IMesh {
    GLuint _vao;
    GLuint _vbo;
    size_t _verticesCount;
public:
    explicit OpenGLMesh(
        const MeshDesc& desc
    ) {
        _verticesCount = desc.vertices.size();

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<long long>(_verticesCount * sizeof(float)),
            desc.vertices.data(),
            GL_STATIC_DRAW
        );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            static_cast<void *>(nullptr)
        );

        glBindVertexArray(0);
    }

    void Draw() override {
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, _verticesCount);
    }
};

}
