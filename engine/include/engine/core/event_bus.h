#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace engine {

class EventBus {
public:
    template <typename Event>
    using Handler = std::function<void(const Event&)>;

    template <typename Event>
    void Subscribe(const Handler<Event>& handler) {
        auto& vec = _handlers[typeid(Event)];
        vec.push_back([handler](const void* e) {
            handler(*static_cast<const Event*>(e));
        });
    }

    template <typename Event, typename... Args>
    void Emit(Args&&... args) {
        Event event{ std::forward<Args>(args)... };

        const auto it = _handlers.find(typeid(Event));
        if (it == _handlers.end())
            return;

        for (auto& function : it->second)
            function(&event);
    }

private:
    std::unordered_map<std::type_index,
        std::vector<std::function<void(const void*)>>> _handlers;
};

}