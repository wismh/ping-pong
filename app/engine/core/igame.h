#pragma once

namespace engine {

class IGame {
public:
    virtual ~IGame() = default;

    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnDraw() = 0;
    virtual void OnQuit() = 0;
};

}