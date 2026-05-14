//
// Created by polyunicorn on 2026/3/26.
//

#ifndef DIFFICULTY_MODEL_H
#define DIFFICULTY_MODEL_H

#include "SceneType.h"
#include "pch.hpp"
#include <array>
#include <string>

struct DifficultyOption {
    DifficultyType difficulty;
    std::string label;
    std::string backgroundImageKey;
    glm::vec2 cardCenter;
    glm::vec2 hitboxSize;
};

class DifficultyModel {
public:
    static int GetBuildCost(
        DifficultyType difficulty,
        const std::string& buildableId
    );

    static int GetUpgradeCost(
        DifficultyType difficulty,
        const std::string& buildableId,
        int upgradePathIndex,
        int nextTier
    );

    static const std::array<DifficultyOption, 3>& GetDifficultyOptions();

    // TODO:
    // Sell refund ratio
    // Income multiplier
};

#endif
