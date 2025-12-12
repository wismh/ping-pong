#include "game/game.h"
#include "game/input_actions.h"

const std::string game::InputActions::PlayerMoveLeft = "player-move-left";
const std::string game::InputActions::PlayerMoveRight = "player-move-right";
const std::string game::InputActions::StartRound = "start-round";

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
