#include "engine/ui/label.h"

namespace engine::ui {

glm::vec2 Label::GetSize(NVGcontext* vg) {
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

void Label::Draw(NVGcontext* vg, const glm::vec2 origin, glm::vec2 space) {
    if (!visible)
        return;

    float height = GetSize(vg).y;

    nvgFontSize(vg, fontSize);
    nvgFontFace(vg, fontName.c_str());
    nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgText(vg, origin.x, origin.y + height, text.c_str(), nullptr);
}

}
