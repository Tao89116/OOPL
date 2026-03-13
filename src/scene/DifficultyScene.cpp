//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/DifficultyScene.h"
#include "scene/SceneManager.h"

void DifficultyScene::Update(SceneManager& sceneManager) {
    m_Controller.HandleInput(sceneManager);
    m_View.Render();
}