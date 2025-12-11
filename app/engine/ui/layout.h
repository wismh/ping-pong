#pragma once
#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ui_element.h"

namespace engine::ui {

    class Layout final : public UIElement {
        std::vector<std::shared_ptr<UIElement>> _children;
    public:
        void Update() override {

        }

        void Draw(NVGcontext* vg, glm::ivec2 origin, glm::ivec2 space) override {

        }
    };

}