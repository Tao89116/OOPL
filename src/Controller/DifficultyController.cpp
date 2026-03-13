//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/DifficultyController.h"

#include "SceneType.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <iostream>
void DifficultyController::HandleInput(SceneManager& sceneManager) {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        sceneManager.SetDifficulty(DifficultyType::Easy);
        sceneManager.RequestSceneChange(SceneType::Game);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        sceneManager.SetDifficulty(DifficultyType::Normal);
        sceneManager.RequestSceneChange(SceneType::Game);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        sceneManager.SetDifficulty(DifficultyType::Hard);
        sceneManager.RequestSceneChange(SceneType::Game);    }

    if (Util::Input::IsKeyUp(Util::Keycode::BACKSPACE)) {
        sceneManager.RequestSceneChange(SceneType::Start);
    }
}