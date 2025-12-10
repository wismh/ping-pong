#pragma once

namespace engine::render {

class IShader {
public:
    virtual ~IShader() = default;
    virtual void Use() = 0;
};

}