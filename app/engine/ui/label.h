#pragma once
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"

namespace engine::ui {

class Label final : public UIElement {
public:
    std::string text;
    glm::vec4 color;
    std::string fontName;
    int fontSize;

    void Update() override {

    }

    void Draw(NVGcontext* vg, glm::ivec2 origin, glm::ivec2 space) override {
        nvgFontSize(vg, fontSize);
        nvgFontFace(vg, fontName.c_str());

        float bounds[4];
        nvgTextBounds(
           vg,
           0, 0,
           text.c_str(),
           nullptr,
           bounds
       );

        origin.y += bounds[3] - bounds[1];

        nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, color.a));
        nvgText(vg, origin.x, origin.y, text.c_str(), nullptr);
    }
};

}