#pragma once

#include "engine/render/itexture.h"
#include "engine/resources/texture_desc.h"
#include "engine/utils.h"

namespace engine::render {

class OpenGLTexture final : public ITexture {
    GLuint _id;
public:
    explicit OpenGLTexture(const TextureDesc& desc);
    ~OpenGLTexture() override;

    void Bind() override;
};

}
