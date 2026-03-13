//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/StartScene.h"
#include "scene/SceneManager.h"

void StartScene::Update(SceneManager& sceneManager) {
    m_Controller.HandleInput(sceneManager);
    m_View.Render();
}