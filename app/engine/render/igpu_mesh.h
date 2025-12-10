#pragma once

namespace engine::render {

class IMesh {
public:
    virtual ~IMesh() = default;
    virtual void Draw() = 0;
};


}