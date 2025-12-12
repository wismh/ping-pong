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

    glm::vec2 GetSize(NVGcontext* vg) override {
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

        return {
            bounds[2] - bounds[0],
            bounds[3] - bounds[1]
        };
    }

    void Draw(NVGcontext* vg, const glm::vec2 origin, glm::vec2 space) override {
        float height = GetSize(vg).y;

        nvgFontSize(vg, fontSize);
        nvgFontFace(vg, fontName.c_str());
        nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, color.a));
        nvgText(vg, origin.x, origin.y + height, text.c_str(), nullptr);
    }
};

}