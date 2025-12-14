#pragma once
#include "engine/ecs/physcis_system.h"
#include "engine/ecs/rigidbody.h"
#include "engine/ecs/transform.h"
#include "engine/audio/audio_events_manager.h"

#include "ball.h"
#include "game_state.h"
#include "player.h"
#include "utils.h"

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
    ) :
        _eventBus(eventBus),
        _time(time),
        _worldBounds(worldBounds),
        _world(world)
    {
        eventBus->Subscribe<ecs::CollisionEvent>([&](const ecs::CollisionEvent& e) {
            OnCollision(e);
        });
    }

    void OnCollision(const ecs::CollisionEvent& e) {
        if (_world.HasComponent<Ball>(e.a) && _world.HasComponent<ecs::RigidBody>(e.a)) {
            const auto& transform = _world.GetComponent<ecs::Transform>(e.a);
            const auto& ball = _world.GetComponent<Ball>(e.a);
            auto& rigidBody = _world.GetComponent<ecs::RigidBody>(e.a);
            auto& player = _world.GetComponent<Player>(e.b);

            player.reflectionCombo += 1;
            rigidBody.velocity.x = transform.position.x > 0 ? -ball.speed : ball.speed;
            _eventBus->Emit<e::PlaySoundEvent>("hit.wav");
        }
    }

    void Update(ecs::World& world) override {
        world.ForEachWith<Ball, ecs::RigidBody, ecs::Transform>([this](
            Ball& ball, ecs::RigidBody& rigidBody, const ecs::Transform& transform
        ) {
            if (transform.position.y > _worldBounds.top) {
                rigidBody.velocity.y = -ball.speed;
                _eventBus->Emit<e::PlaySoundEvent>("hit.wav");
            }

            if (transform.position.y < _worldBounds.bottom) {
                rigidBody.velocity.y = ball.speed;
                _eventBus->Emit<e::PlaySoundEvent>("hit.wav");
            }
        });
    }
};

}