#pragma once

#include <nanovg.h>

namespace engine::ui {

class UIElement {
public:
    bool visible = true;
    bool relative = true;

    virtual ~UIElement() = default;
    virtual glm::vec2 GetSize(NVGcontext* vg) = 0;
    virtual void Draw(NVGcontext *vg, glm::vec2 origin, glm::vec2 space) = 0;

    virtual bool OnMouseDown(const glm::vec2 position) { return false; }
    virtual bool OnMouseUp(const glm::vec2 position) { return false; }
    virtual bool OnMouseMove(const glm::vec2 position) { return false; }
};

}