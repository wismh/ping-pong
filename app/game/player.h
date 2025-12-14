#pragma once

namespace game {

enum class PlayerControlBy {
    BLUE_ACTIONS,
    RED_ACTIONS,
    AI
};

struct Player {
    float speed = 15;
    PlayerControlBy control = PlayerControlBy::BLUE_ACTIONS;
    int reflectionCombo = 0;
};

}