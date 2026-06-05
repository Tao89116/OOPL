//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/SceneManager.h"

#include "ResourceManager.h"
#include "scene/StartScene.h"
#include "scene/DifficultyScene.h"
#include "scene/GameScene.h"
#include "scene/ResultScene.h"

SceneManager::SceneManager() {
    ResourceManager::GetInstance().Initialize();
    CreateScene(SceneType::Start);
}

void SceneManager::ChangeScene(SceneType sceneType) {
    m_CurrentSceneType = sceneType;
    CreateScene(sceneType);
}

void SceneManager::RequestSceneChange(SceneType sceneType) {
    m_HasPendingSceneChange = true;
    m_PendingSceneType = sceneType;
}

void SceneManager::CreateScene(SceneType sceneType) {
    switch (sceneType) {
        case SceneType::Start:
            m_CurrentScene = std::make_unique<StartScene>();
            break;
        case SceneType::Difficulty:
            m_CurrentScene = std::make_unique<DifficultyScene>();
            break;
        case SceneType::Game:
            if (!m_GameSession) {
                m_GameSession = std::make_shared<GameModel>(m_Difficulty);
            }
            m_CurrentScene = std::make_unique<GameScene>(m_Difficulty, m_GameSession);
            break;
        case SceneType::Result:
            m_GameSession.reset();
            m_CurrentScene = std::make_unique<ResultScene>(m_Result);
            break;
    }
}

void SceneManager::Update() {
    if (m_CurrentScene) {
        m_CurrentScene->Update(*this);
    }

    if (m_HasPendingSceneChange) {
        m_HasPendingSceneChange = false;
        ChangeScene(m_PendingSceneType);
    }
}
