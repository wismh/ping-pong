#include "engine/render/backends/opengl/opengl_shader.h"

#include "engine/resources/resource_create_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace engine::render {

OpenGLShader::OpenGLShader(
   const ShaderDesc& desc
) {
    auto compileShader = [this](const GLenum type, const char* src) -> GLuint {
        const auto shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            throw ResourceCreateException(
                "Shader",
                fmt::format("Compilation error: {}", infoLog)
            );
        }

        return shader;
    };

    const auto vertexShader = compileShader(GL_VERTEX_SHADER, desc.vertexShaderSrc);
    const auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, desc.fragmentShaderSrc);

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    GLint success;
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(_program, 512, nullptr, infoLog);
        throw ResourceCreateException(
            "Shader",
            fmt::format("Program linking error: {}", infoLog)
        );
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLShader::Use() {
    glUseProgram(_program);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(
        glGetUniformLocation(_program, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

}