#include "game/game.h"

int main() {
    engine::Engine<game::Game> engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
