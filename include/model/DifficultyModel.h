//
// Created by polyunicorn on 2026/3/26.
//

#ifndef DIFFICULTY_MODEL_H
#define DIFFICULTY_MODEL_H

#include "SceneType.h"
#include <string>

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

    // TODO:
    // Sell refund ratio
    // Income multiplier
};

#endif