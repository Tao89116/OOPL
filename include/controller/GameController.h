//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "model/GameModel.h"
#include <array>

class GameController {
public:
    void HandleInput(GameModel& model);

private:
    enum class CheatStep {
        Up,
        Down,
        Left,
        Right,
        B,
        A
    };

    bool ConsumeCheatSequenceInput(GameModel& model);
    void HandleCheatModeInput(GameModel& model);

    std::array<CheatStep, 10> m_CheatBuffer{};
    std::size_t m_CheatWriteIndex = 0;
    std::size_t m_CheatCount = 0;
    int m_CheatDifficultyIndex = 1;
    int m_CheatEnemyTypeIndex = 0;
    int m_CheatSpawnCount = 1;
};

#endif
