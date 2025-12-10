#pragma once

#include "node.h"
#include "../ecs/ecs.h"

namespace engine {

class NodeEcs final : public Node {
    ecs::World world;
public:
    explicit NodeEcs(const std::shared_ptr<Node>& parent)
        : Node(parent) {
    }

    ecs::World& GetWorld() {
        return world;
    }

protected:
    void UpdateSelf() override {
        world.Update();
    }
};

}