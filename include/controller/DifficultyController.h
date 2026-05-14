//
// Created by polyunicorn on 2026/3/13.
//

#ifndef DIFFICULTY_CONTROLLER_H
#define DIFFICULTY_CONTROLLER_H

#include "model/DifficultyModel.h"

class SceneManager;

class DifficultyController {
public:
    void HandleInput(SceneManager& sceneManager);

private:
    static bool IsCursorInsideCard(const glm::vec2& cursorPosition, const DifficultyOption& option);
    static void StartGameWithDifficulty(SceneManager& sceneManager, DifficultyType difficulty);
};

#endif
