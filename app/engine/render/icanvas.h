#pragma once

namespace engine::render {

class ICanvas {
public:
    virtual ~ICanvas() = default;
    virtual bool Init() = 0;
    virtual void Draw() = 0;
    virtual void Dispose() = 0;
};

}