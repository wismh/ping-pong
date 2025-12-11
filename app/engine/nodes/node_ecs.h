#pragma once

#include "node.h"
#include "../ecs/ecs.h"

namespace engine {

class NodeEcs final : public Node {
    ecs::World world;
public:
    ecs::World& GetWorld() {
        return world;
    }

protected:
    void UpdateSelf() override {
        world.Update();
    }
};

}