#pragma once
#include "engine/ui/ui_element.h"
#include "engine/utils.h"

#include "igpu_mesh.h"
#include "ishader.h"
#include "itexture.h"

namespace engine::render {

struct CmdDrawMesh {
    std::shared_ptr<ITexture> texture;
    std::shared_ptr<IShader> shader;
    std::shared_ptr<IMesh> mesh;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

struct CmdCustomDraw {
    std::function<void()> draw;
};

struct CmdDrawUI {
    std::shared_ptr<ui::UIElement> uiElement;
};

using Command = std::variant<
    CmdDrawMesh,
    CmdCustomDraw,
    CmdDrawUI
>;

}
