#include "scene/StartScene.h"

#include "SceneType.h"
#include "Util/Time.hpp"
#include "scene/SceneManager.h"

void StartScene::Update(SceneManager& sceneManager) {
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();
    const int poppedCount = m_Controller.Update(m_Model, deltaTimeMs);

    m_View.PlayPopSounds(poppedCount);
    m_View.Render(m_Model);

    if (m_Model.IsAnimationComplete()) {
        sceneManager.RequestSceneChange(SceneType::Difficulty);
    }
}
