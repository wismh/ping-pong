#include "engine/render/backends/opengl/opengl_texture.h"

namespace engine::render {

OpenGLTexture::OpenGLTexture(const TextureDesc& desc) {
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        desc.width, desc.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        desc.data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture::~OpenGLTexture() {
    if (_id)
        glDeleteTextures(1, &_id);
}

void OpenGLTexture::Bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _id);
}

}