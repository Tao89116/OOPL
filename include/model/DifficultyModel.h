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
    // 之後若要做更完整的平衡表，可再補：
    // - Sell refund ratio
    // - Income multiplier
    // - Special mode modifiers
};

#endif