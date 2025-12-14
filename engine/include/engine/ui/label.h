#pragma once
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"

namespace engine::ui {

class Label final : public UIElement {
public:
    std::string text;
    glm::i8vec4 color;
    std::string fontName;
    float fontSize;

    glm::vec2 GetSize(NVGcontext* vg) override;
    void Draw(NVGcontext* vg, const glm::vec2 origin, glm::vec2 space) override;
};
    
}