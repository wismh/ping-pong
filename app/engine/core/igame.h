#pragma once

namespace engine {

class IGame {
public:
    virtual ~IGame() = default;

    virtual void OnStart() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnDraw(float deltaTime) = 0;
    virtual void OnQuit() = 0;
};

}