#pragma once

namespace game {

struct GameState {
    int bluePlayerScore = 0;
    int redPlayerScore = 0;
    bool paused = true;
    bool isBlueTurn = true;
};

}