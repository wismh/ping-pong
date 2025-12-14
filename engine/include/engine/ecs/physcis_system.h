#pragma once
#include <unordered_set>

#include "engine/core/event_bus.h"
#include "engine/core/time.h"

#include "ecs.h"
#include "rigidbody.h"

namespace engine::ecs {

struct CollisionEvent {
    Entity a;
    Entity b;
};

class PhysicsSystem final : public ISystem {
    struct PairHash {
        size_t operator()(const std::pair<Entity, Entity>& p) const noexcept;
    };

    std::unordered_set<std::pair<Entity, Entity>, PairHash> lastSet;
    std::shared_ptr<EventBus> _eventBus;
    std::shared_ptr<Time> _time;
public:
    explicit PhysicsSystem(
        const std::shared_ptr<EventBus>& eventBus,
        const std::shared_ptr<Time>& time
    );

    void Update(World& world) override;
private:
    void Move(World& world);
    void Collisions(World& world);

    static std::pair<Entity, Entity> MakePair(Entity a, Entity b);
};

}