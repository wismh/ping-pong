#pragma once
#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"

namespace engine::ui {

    enum class Align { Start, Center, End };
    enum class Direction { Horizontal, Vertical };

    class Layout final : public UIElement {
    public:
        glm::vec2 size = {};
        glm::uvec4 color = { 0, 0, 0, 0 };
        Align align = Align::Start;
        Align crossAlign = Align::Start;
        Direction direction = Direction::Horizontal;
        std::vector<std::shared_ptr<UIElement>> children = {};

        glm::vec2 GetSize(NVGcontext* vg) override {
            return size;
        }

        void Draw(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) override {
            DrawSelf(vg, origin, space);
            DrawChildren(vg, origin, space);
        }
    private:
        void DrawSelf(NVGcontext* vg, const glm::vec2 origin, const glm::vec2 space) const {
            nvgBeginPath(vg);
            nvgRect(vg, origin.x, origin.y, size.x, size.y);
            nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, color.a));
            nvgFill(vg);
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

                if (direction == Direction::Horizontal) {
                    totalMainSize += size.x;
                    maxCrossSize = std::max(maxCrossSize, size.y);
                } else {
                    totalMainSize += size.y;
                    maxCrossSize = std::max(maxCrossSize, size.x);
                }
            }

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

            for (size_t i = 0; i < children.size(); i++)
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

                child->Draw(vg, childPosition, size);

                if (direction == Direction::Horizontal)
                    position.x += size.x;
                else
                    position.y += size.y;
            }
        }
    };

}