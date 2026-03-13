#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "scene/SceneManager.h"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    State m_CurrentState = State::START;
    std::unique_ptr<SceneManager> m_SceneManager = nullptr;
};


#endif
