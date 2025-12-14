#pragma once
#include "engine/ecs/ecs.h"
#include "node.h"

namespace engine {

class NodeEcs final : public Node {
    ecs::World world;
public:
    ecs::World& GetWorld();

protected:
    void UpdateSelf() override;
};

}