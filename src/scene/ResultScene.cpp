//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/ResultScene.h"

#include "scene/SceneManager.h"
#include "Util/Time.hpp"

ResultScene::ResultScene(ResultType result)
    : m_Model(result), m_View(result) {
}

void ResultScene::Update(SceneManager& sceneManager) {
    if (!m_Started) {
        m_Model.Start();
        m_View.PlayResultSoundOnce();
        m_Started = true;
    }

    m_Model.Update(Util::Time::GetDeltaTimeMs());

    m_View.Render(m_Model);
    m_Controller.Update(m_Model, sceneManager);
}
