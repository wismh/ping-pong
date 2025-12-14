#include "engine/render/backends/opengl/opengl_mesh.h"

namespace engine::render {

OpenGLMesh::OpenGLMesh(
    const MeshDesc& desc
) {
    _verticesCount = desc.vertices.size();

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<long long>(_verticesCount * sizeof(Vertex)),
        desc.vertices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        static_cast<void *>(nullptr)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void *>(offsetof(Vertex, uv))
    );

    glBindVertexArray(0);
}

void OpenGLMesh::Draw() {
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _verticesCount);
}

}