#pragma once

namespace game {

struct AiParams {
    float baseReactionTime = 0.3f;
    float increaseReactionTimeByReflection = 0.05f;
    float decreaseReactionTimeByScore = 0.05f;
    float difficulty = 5;
};

}