#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("App Start");
    m_SceneManager = std::make_unique<SceneManager>();
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if (!m_SceneManager) {
        m_CurrentState = State::END;
        return;
    }

    m_SceneManager->Update();

    if (Util::Input::IfExit() || Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::END;
    }
}

void App::End() {
    LOG_TRACE("App End");
    m_SceneManager.reset();
}
