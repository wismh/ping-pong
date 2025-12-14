#pragma once
#include <memory>

#include "engine/utils.h"
#include "SDL3/SDL_scancode.h"
#include "event_bus.h"

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
    explicit InputSystem(const std::shared_ptr<EventBus>& eventBus);

    void AddBinding(SDL_Scancode key, const std::string& action);
    void ProcessEvent(const SDL_Event& e);
private:
    void ProcessKeyDown(const SDL_Event& e);
    void ProcessMouseDown(const SDL_Event& e);
    void ProcessMouseUp(const SDL_Event& e);
    void ProcessMouseMove(const SDL_Event& e);
};

}