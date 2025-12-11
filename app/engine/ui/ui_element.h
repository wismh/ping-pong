#pragma once

#include <nanovg.h>

namespace engine::ui {

class UIElement {
public:
    virtual ~UIElement() = default;
    virtual void Update() = 0;
    virtual void Draw(NVGcontext* vg, glm::ivec2 origin, glm::ivec2 space) = 0;
};

}