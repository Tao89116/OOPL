//
// Created by polyunicorn on 2026/3/13.
//

#ifndef DIFFICULTY_SCENE_H
#define DIFFICULTY_SCENE_H

#include "scene/IScene.h"
#include "view/DifficultyView.h"
#include "controller/DifficultyController.h"
#include <array>

class DifficultyScene : public IScene {
public:
    enum class CheatStep { Up, Down, Left, Right, B, A };
    DifficultyScene() = default;
    void Update(SceneManager& sceneManager) override;

private:
    bool ConsumeCheatSequenceInput();

    DifficultyView m_View;
    DifficultyController m_Controller;
    std::array<CheatStep, 10> m_CheatBuffer{};
    std::size_t m_CheatWriteIndex = 0;
    std::size_t m_CheatCount = 0;
};

#endif
