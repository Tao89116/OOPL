//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/ResultScene.h"

#include "scene/SceneManager.h"
#include "Util/Time.hpp"

ResultScene::ResultScene(
    ResultType result,
    DifficultyType difficulty,
    const std::shared_ptr<GameModel>& finishedGame
)
    : m_Model(result), m_View(result), m_FinishedGame(finishedGame) {
    if (m_FinishedGame) {
        m_GameBackdrop = std::make_unique<GameView>(difficulty);
        m_GameBackdrop->Initialize(*m_FinishedGame);
    }
}

void ResultScene::Update(SceneManager& sceneManager) {
    if (!m_Started) {
        m_Model.Start();
        m_View.PlayResultSoundOnce();
        m_Started = true;
    }

    m_Model.Update(Util::Time::GetDeltaTimeMs());

    if (m_GameBackdrop && m_FinishedGame) {
        m_GameBackdrop->Render(*m_FinishedGame);
    }
    m_View.Render(m_Model);
    m_Controller.Update(m_Model, sceneManager);
}
