//
// Created by polyunicorn on 2026/3/13.
//
#include "scene/ResultScene.h"
#include "scene/SceneManager.h"

ResultScene::ResultScene(ResultType result)
    : m_View(result) {
}

void ResultScene::Update(SceneManager& sceneManager) {
    m_Controller.HandleInput(sceneManager);
    m_View.Render();
}