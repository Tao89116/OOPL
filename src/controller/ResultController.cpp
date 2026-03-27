//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/ResultController.h"

#include "SceneType.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void ResultController::HandleInput(SceneManager& sceneManager) {
    if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
        sceneManager.RequestSceneChange(SceneType::Start);
    }
}