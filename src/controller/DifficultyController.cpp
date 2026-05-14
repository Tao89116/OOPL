//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/DifficultyController.h"

#include "SceneType.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

bool DifficultyController::IsCursorInsideCard(
    const glm::vec2& cursorPosition,
    const DifficultyOption& option
) {
    const glm::vec2 halfSize = option.hitboxSize * 0.5f;
    return cursorPosition.x >= option.cardCenter.x - halfSize.x &&
           cursorPosition.x <= option.cardCenter.x + halfSize.x &&
           cursorPosition.y >= option.cardCenter.y - halfSize.y &&
           cursorPosition.y <= option.cardCenter.y + halfSize.y;
}

void DifficultyController::StartGameWithDifficulty(
    SceneManager& sceneManager,
    DifficultyType difficulty
) {
    sceneManager.SetDifficulty(difficulty);
    sceneManager.RequestSceneChange(SceneType::Game);
}

void DifficultyController::HandleInput(SceneManager& sceneManager) {
    if (Util::Input::IsKeyUp(Util::Keycode::BACKSPACE)) {
        sceneManager.RequestSceneChange(SceneType::Start);
        return;
    }

    const glm::vec2 cursorPosition = Util::Input::GetCursorPosition();
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        for (const auto& option : DifficultyModel::GetDifficultyOptions()) {
            if (IsCursorInsideCard(cursorPosition, option)) {
                StartGameWithDifficulty(sceneManager, option.difficulty);
                return;
            }
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        StartGameWithDifficulty(sceneManager, DifficultyType::Easy);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        StartGameWithDifficulty(sceneManager, DifficultyType::Normal);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        StartGameWithDifficulty(sceneManager, DifficultyType::Hard);
    }
}
