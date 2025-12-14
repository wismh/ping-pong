#include "engine/nodes/node_ecs.h"

namespace engine {

ecs::World& NodeEcs::GetWorld() {
    return world;
}

void NodeEcs::UpdateSelf() {
    world.Update();
}

}