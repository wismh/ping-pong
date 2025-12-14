#pragma once
#include "../utils.h"

namespace engine::render {

class IShader {
public:
    virtual ~IShader() = default;

    virtual void Use() = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
};

}