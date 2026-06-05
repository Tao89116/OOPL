//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/ResultController.h"

#include "SceneType.h"
#include "model/ResultModel.h"
#include "scene/SceneManager.h"

void ResultController::Update(ResultModel& model, SceneManager& sceneManager) {
    if (m_ReturnRequested || !model.IsFinished()) {
        return;
    }

    sceneManager.SetGameSession(nullptr);
    sceneManager.RequestSceneChange(SceneType::Difficulty);
    m_ReturnRequested = true;
}
