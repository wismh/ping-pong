#include "game/game.h"
#include "game/input_actions.h"

const std::string game::InputActions::BluePlayerMoveLeft = "blue-player-move-left";
const std::string game::InputActions::BluePlayerMoveRight = "blue-player-move-right";
const std::string game::InputActions::RedPlayerMoveLeft = "red-player-move-left";
const std::string game::InputActions::RedPlayerMoveRight = "red-player-move-right";
const std::string game::InputActions::StartRound = "start-round";

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
