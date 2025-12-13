#pragma once

namespace game {

struct Bounds {
    float left, right, top, bottom;
};

struct GameState {
    int bluePlayerScore = 0;
    int redPlayerScore = 0;
    bool paused = true;
    bool isBlueTurn = true;
};

}