#include "engine/core/engine.h"

int main() {
    engine::Engine engine{};

    if (!engine.Init())
        return EXIT_FAILURE;

    return engine.Run();
}
