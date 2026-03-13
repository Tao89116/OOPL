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
            m_CurrentScene = std::make_unique<GameScene>(m_Difficulty);
            break;
        case SceneType::Result:
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
// IMPORTANT:
// Do not switch scene immediately during a scene's Update(),
// because the current scene object may still be executing.
// Use RequestSceneChange() and apply it after Update() finishes.