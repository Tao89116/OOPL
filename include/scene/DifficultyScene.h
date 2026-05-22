//
// Created by polyunicorn on 2026/3/13.
//

#ifndef DIFFICULTY_SCENE_H
#define DIFFICULTY_SCENE_H

#include "scene/IScene.h"
#include "view/DifficultyView.h"
#include "controller/DifficultyController.h"
#include "model/GameModel.h"
#include <array>

class DifficultyScene : public IScene {
public:
    enum class CheatStep { Up, Down, Left, Right, B, A };
    DifficultyScene() = default;
    void Update(SceneManager& sceneManager) override;

private:
    bool ConsumeCheatSequenceInput();
    void DrawCheatGui(SceneManager& sceneManager);

    DifficultyView m_View;
    DifficultyController m_Controller;
    bool m_CheatMode = false;
    std::array<CheatStep, 10> m_CheatBuffer{};
    std::size_t m_CheatWriteIndex = 0;
    std::size_t m_CheatCount = 0;
    std::shared_ptr<GameModel> m_CheatModel = nullptr;

    int m_GoldInput = 650;
    int m_HPInput = 100;
    int m_RoundInput = 1;
    int m_EnemyTypeIndex = 0;
    int m_SpawnCountInput = 1;
};

#endif
