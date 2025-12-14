#include "engine/core/input_system.h"

namespace engine {

InputSystem::InputSystem(const std::shared_ptr<EventBus>& eventBus)
    : _eventBus(eventBus) {}

void InputSystem::AddBinding(SDL_Scancode key, const std::string& action) {
    _bindings.push_back({ key, action });
}

void InputSystem::ProcessEvent(const SDL_Event& e) {
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

void InputSystem::ProcessKeyDown(const SDL_Event& e) {
    const auto sc = e.key.scancode;

    for (auto& [key, action] : _bindings) {
        if (key == sc)
            _eventBus->Emit<InputEvent>(action);
    }
}

void InputSystem::ProcessMouseDown(const SDL_Event& e) {
    _eventBus->Emit<MouseEvent>(MouseEvent{
       .type = MouseEvent::Type::Down,
       .position  = {
            static_cast<float>(e.button.x),
            static_cast<float>(e.button.y)
       },
       .button = e.button.button
   });
}

void InputSystem::ProcessMouseUp(const SDL_Event& e) {
    _eventBus->Emit<MouseEvent>(MouseEvent{
       .type =  MouseEvent::Type::Up,
       .position = {
            static_cast<float>(e.button.x),
            static_cast<float>(e.button.y)
        },
       .button = e.button.button
    });
}

void InputSystem::ProcessMouseMove(const SDL_Event& e) {
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

}