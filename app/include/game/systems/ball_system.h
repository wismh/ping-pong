#pragma once
#include "engine/ecs/physcis_system.h"

#include "game/components/game_state.h"
#include "game/utils.h"

namespace game {

class BallSystem final : public ecs::ISystem {
    std::shared_ptr<e::EventBus> _eventBus;
    std::shared_ptr<e::Time> _time;
    Bounds& _worldBounds;
    ecs::World& _world;
public:
    BallSystem(
        const std::shared_ptr<e::EventBus>& eventBus,
        const std::shared_ptr<e::Time>& time,
        ecs::World& world,
        Bounds& worldBounds
    );

    void OnCollision(const ecs::CollisionEvent& e);
    void Update(ecs::World& world) override;
};

}