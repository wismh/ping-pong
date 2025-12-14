#pragma once
#include <memory>

#include "engine/resources/ui_image.h"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"

namespace engine::ui {

    enum class Align { Start, Center, End };
    enum class Direction { Horizontal, Vertical };

    class Layout final : public UIElement {
        glm::vec2 _lastOrigin = {};
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

        glm::vec2 GetSize(NVGcontext* vg) override;

        void Draw(NVGcontext* vg, glm::vec2 origin, glm::vec2 space) override;
        bool OnMouseDown(glm::vec2 position) override;
    private:
        bool Hit(glm::vec2 p) const;

        void DrawSelf(NVGcontext* vg, glm::vec2 origin, glm::vec2 space) const;
        void DrawChildren(NVGcontext* vg, glm::vec2 origin, glm::vec2 space) const;
    };

}