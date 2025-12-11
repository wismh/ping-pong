#pragma once
#include <memory>

#include "SDL3/SDL_scancode.h"
#include "event_bus.h"
#include "../utils.h"

namespace engine {

struct InputEvent {
    std::string action;
};

struct InputBinding {
    SDL_Scancode key;
    std::string action;
};

class InputSystem {
public:
    explicit InputSystem(const std::shared_ptr<EventBus>& eventBus)
        : _eventBus(eventBus) {}

    void AddBinding(SDL_Scancode key, const std::string& action) {
        _bindings.push_back({ key, action });
    }

    void ProcessEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_KEY_DOWN) {
            const auto sc = e.key.scancode;

            for (auto& [key, action] : _bindings) {
                if (key == sc)
                    _eventBus->Emit<InputEvent>(action);
            }
        }
    }

private:
    std::vector<InputBinding> _bindings;
    std::shared_ptr<EventBus> _eventBus;
};

}