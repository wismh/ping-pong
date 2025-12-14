#pragma once
#include "engine/render/ishader.h"
#include "engine/resources/shader_desc.h"

namespace engine::render {

class OpenGLShader final : public IShader {
    GLuint _program;
public:
    explicit OpenGLShader(const ShaderDesc& desc);

    void Use() override;
    void SetMat4(const std::string &name, const glm::mat4 &value) override;
};

}
