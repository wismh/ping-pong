#include "game/game.h"
#include "game/input_actions.h"

const std::string game::InputActions::PlayerMoveLeft = "player-move-left";
const std::string game::InputActions::PlayerMoveRight = "player-move-right";

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
