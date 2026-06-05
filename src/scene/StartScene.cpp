#include "scene/StartScene.h"

#include "SceneType.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "scene/SceneManager.h"

void StartScene::Update(SceneManager& sceneManager) {
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();
    m_Controller.Update(m_Model, deltaTimeMs);
    const auto poppedEvents = m_Model.ConsumePoppedBloonEvents();

    m_View.QueuePopEffects(poppedEvents);
    m_View.PlayPopSounds(static_cast<int>(poppedEvents.size()));
    m_View.Render(m_Model);

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
        m_View.IsPlayButtonHit(Util::Input::GetCursorPosition())) {
        sceneManager.RequestSceneChange(SceneType::Difficulty);
        return;
    }

    if (m_Model.IsAnimationComplete()) {
        sceneManager.RequestSceneChange(SceneType::Difficulty);
    }
}
