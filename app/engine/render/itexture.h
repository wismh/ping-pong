#pragma once

namespace engine::render {

class ITexture {
public:
    virtual ~ITexture() = default;
    virtual void Bind() = 0;
};

}