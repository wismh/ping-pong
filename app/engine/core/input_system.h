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

struct MouseEvent {
    enum class Type {
        Down, Up, Move
    } type;

    glm::vec2 position {};
    glm::vec2 relative {};
    uint8_t button = 0;
};
    
class InputSystem {
    std::vector<InputBinding> _bindings;
    std::shared_ptr<EventBus> _eventBus;
public:
    explicit InputSystem(const std::shared_ptr<EventBus>& eventBus)
        : _eventBus(eventBus) {}

    void AddBinding(SDL_Scancode key, const std::string& action) {
        _bindings.push_back({ key, action });
    }

    void ProcessEvent(const SDL_Event& e) {
        switch (e.type) {
            case SDL_EVENT_KEY_DOWN:
                ProcessKeyDown(e);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                ProcessMouseDown(e);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                ProcessMouseUp(e);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                ProcessMouseMove(e);
                break;
            default:
                break;
        }
    }
private:
    void ProcessKeyDown(const SDL_Event& e) {
        const auto sc = e.key.scancode;

        for (auto& [key, action] : _bindings) {
            if (key == sc)
                _eventBus->Emit<InputEvent>(action);
        }
    }

    void ProcessMouseDown(const SDL_Event& e) {
        _eventBus->Emit<MouseEvent>(MouseEvent{
           .type = MouseEvent::Type::Down,
           .position  = {
                static_cast<float>(e.button.x),
                static_cast<float>(e.button.y)
           },
           .button = e.button.button
       });
    }

    void ProcessMouseUp(const SDL_Event& e) {
        _eventBus->Emit<MouseEvent>(MouseEvent{
           .type =  MouseEvent::Type::Up,
           .position = {
                static_cast<float>(e.button.x),
                static_cast<float>(e.button.y)
            },
           .button = e.button.button
        });
    }

    void ProcessMouseMove(const SDL_Event& e) {
        _eventBus->Emit<MouseEvent>(MouseEvent{
            .type = MouseEvent::Type::Move,
            .position = {
                static_cast<float>(e.motion.x),
                static_cast<float>(e.motion.y)
            },
            .relative  = {
                static_cast<float>(e.motion.xrel),
                static_cast<float>(e.motion.yrel)
            }
        });
    }
};

}