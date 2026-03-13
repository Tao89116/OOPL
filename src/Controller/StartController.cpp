//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/StartController.h"

#include "SceneType.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <iostream>


void StartController::HandleInput(SceneManager& sceneManager) {
    if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
        std::cout << "Enter pressed -> DifficultyScene" << std::endl;
        sceneManager.RequestSceneChange(SceneType::Difficulty);
    }
}