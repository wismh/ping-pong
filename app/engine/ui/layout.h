#pragma once
#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"
#include "../resources/ui_image.h"

namespace engine::ui {

    enum class Align { Start, Center, End };
    enum class Direction { Horizontal, Vertical };

    class Layout final : public UIElement {
        glm::vec2 lastOrigin;
    public:
        glm::vec2 size = {};
        glm::uvec4 color = { };
        glm::vec2 gap = {};
        float radius = 0;
        float strokeWidth = 0;
        glm::uvec4 strokeColor = {};
        std::shared_ptr<UIImage> image = nullptr;
        Align align = Align::Start;
        Align crossAlign = Align::Start;
        Direction direction = Direction::Horizontal;
        std::vector<std::shared_ptr<UIElement>> children = {};
        std::function<void()> onClick = nullptr;
        bool clickable = false;

        glm::vec2 GetSize(NVGcontext* vg) override {
            return size;
        }

        void Draw(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) override {
            if (!visible)
                return;

            lastOrigin = origin;
            DrawSelf(vg, origin, space);
            DrawChildren(vg, origin, space);
        }

        bool OnMouseDown(glm::vec2 position) override {
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
    private:
        bool Hit(const glm::vec2 p) const {
            return
                p.x >= lastOrigin.x &&
                p.y >= lastOrigin.y &&
                p.x <= lastOrigin.x + size.x &&
                p.y <= lastOrigin.y + size.y;
        }

        void DrawSelf(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) const {
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

        void DrawChildren(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) const {
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
    };

}