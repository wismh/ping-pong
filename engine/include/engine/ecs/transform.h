#pragma once
#include "engine/utils.h"

namespace engine::ecs {

struct Transform {
    glm::vec3 position{0};
    glm::vec3 rotation{0};
    glm::vec3 scale{1};
};

}