#include "engine/ui/layout.h"

namespace engine::ui {

glm::vec2 Layout::GetSize(NVGcontext* vg) {
    return size;
}

void Layout::Draw(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) {
    if (!visible)
        return;

    _lastOrigin = origin;
    DrawSelf(vg, origin, space);
    DrawChildren(vg, origin, space);
}

bool Layout::OnMouseDown(glm::vec2 position) {
    if (!visible)
        return false;

    for (auto it = children.rbegin(); it != children.rend(); ++it)
        if ((*it)->OnMouseDown(position))
            return true;

    if (clickable && Hit(position)) {
        if (onClick)
            onClick();
        return true;
    }

    return false;
}

bool Layout::Hit(const glm::vec2 p) const {
    return
        p.x >= _lastOrigin.x &&
        p.y >= _lastOrigin.y &&
        p.x <= _lastOrigin.x + size.x &&
        p.y <= _lastOrigin.y + size.y;
}

void Layout::DrawSelf(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) const {
    nvgBeginPath(vg);
    nvgRoundedRect(vg, origin.x, origin.y, size.x, size.y, radius);

    if (image != nullptr) {
        NVGpaint bg = nvgImagePattern(
            vg,
            origin.x, origin.y, size.x, size.y,
            0.0f,
            image->image,
            1.0f
        );
        nvgFillPaint(vg, bg);
    } else
        nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, color.a));

    nvgFill(vg);

    nvgStrokeWidth(vg, strokeWidth);
    nvgStrokeColor(vg, nvgRGBA(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a));
    nvgStroke(vg);
}

void Layout::DrawChildren(NVGcontext *vg, const glm::vec2 origin, const glm::vec2 space) const {
    std::vector<glm::vec2> sizes;
    sizes.reserve(children.size());

    float totalMainSize = 0.0f;
    float maxCrossSize = 0.0f;

    for (const auto& child : children)
    {
        glm::vec2 size = child->GetSize(vg);
        sizes.push_back(size);

        if (!child->relative)
            continue;

        if (direction == Direction::Horizontal) {
            totalMainSize += size.x + gap.x;
            maxCrossSize = std::max(maxCrossSize, size.y);
        } else {
            totalMainSize += size.y + gap.y;
            maxCrossSize = std::max(maxCrossSize, size.x);
        }
    }

    if (children.size() > 0)
        totalMainSize -= direction == Direction::Horizontal ? gap.x : gap.y;

    auto position = origin;

    float remainingMain = (direction == Direction::Horizontal ? space.x : space.y) - totalMainSize;
    remainingMain = std::max(remainingMain, 0.0f);

    if (align == Align::Center) {
        if (direction == Direction::Horizontal)
            position.x += remainingMain * 0.5f;
        else
            position.y += remainingMain * 0.5f;
    } else if (align == Align::End) {
        if (direction == Direction::Horizontal)
            position.x += remainingMain;
        else
            position.y += remainingMain;
    }

    for (size_t i = 0; i < children.size(); ++i)
    {
        const auto& child = children[i];

        glm::vec2 size = sizes[i];
        glm::vec2 childPosition = position;

        float remainingCross =
            (direction == Direction::Horizontal ? space.y : space.x) - (direction == Direction::Horizontal ? size.y : size.x);
        remainingCross = std::max(remainingCross, 0.0f);

        if (crossAlign == Align::Center) {
            if (direction == Direction::Horizontal)
                childPosition.y += remainingCross * 0.5f;
            else
                childPosition.x += remainingCross * 0.5f;
        } else if (crossAlign == Align::End) {
            if (direction == Direction::Horizontal)
                childPosition.y += remainingCross;
            else
                childPosition.x += remainingCross;
        }

        if (!child->relative)
            childPosition = origin;

        child->Draw(vg, childPosition, size);

        if (child->relative) {
            if (direction == Direction::Horizontal)
                position.x += size.x + gap.x;
            else
                position.y += size.y + gap.y;
        }
    }
}

}