#include "model/DifficultyModel.h"

namespace {
const std::array<DifficultyOption, 3> kDifficultyOptions = {{
    {DifficultyType::Easy,   "Easy",   "bg_easy",   {-360.0f, 0.0f}, {280.0f, 340.0f}},
    {DifficultyType::Normal, "Normal", "bg_normal", {   0.0f, 0.0f}, {280.0f, 340.0f}},
    {DifficultyType::Hard,   "Hard",   "bg_hard",   { 360.0f, 0.0f}, {280.0f, 340.0f}},
}};
}


int DifficultyModel::GetBuildCost(
    DifficultyType difficulty,
    const std::string& buildableId
) {
    if (buildableId == "dart_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 250;
            case DifficultyType::Normal: return 280;
            case DifficultyType::Hard:   return 285;
        }
    }

    if (buildableId == "track_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 340;
            case DifficultyType::Normal: return 385;
            case DifficultyType::Hard:   return 390;
        }
    }

    if (buildableId == "cannon_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 520;
            case DifficultyType::Normal: return 585;
            case DifficultyType::Hard:   return 600;
        }
    }

    if (buildableId == "glue_trap") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 35;
            case DifficultyType::Normal: return 40;
            case DifficultyType::Hard:   return 40;
        }
    }

    if (buildableId == "super_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 3600;
            case DifficultyType::Normal: return 4080;
            case DifficultyType::Hard:   return 4160;
        }
    }

    if (buildableId == "iceball_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 385;
            case DifficultyType::Normal: return 435;
            case DifficultyType::Hard:   return 440;
        }
    }

    if (buildableId == "spike_trap") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 25;
            case DifficultyType::Normal: return 30;
            case DifficultyType::Hard:   return 30;
        }
    }

    if (buildableId == "boomerang_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 475;
            case DifficultyType::Normal: return 535;
            case DifficultyType::Hard:   return 545;
        }
    }

    return 999999;
}

int DifficultyModel::GetUpgradeCost(
    DifficultyType difficulty,
    const std::string& buildableId,
    int upgradePathIndex,
    int nextTier
) {
    if (nextTier != 1 || upgradePathIndex < 0 || upgradePathIndex > 1) {
        return 999999;
    }

    auto byDifficulty = [difficulty](int easy, int normal, int hard) {
        switch (difficulty) {
            case DifficultyType::Easy:   return easy;
            case DifficultyType::Normal: return normal;
            case DifficultyType::Hard:   return hard;
        }
        return 999999;
    };

    if (buildableId == "dart_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(180, 205, 210)
            : byDifficulty(80, 90, 95);
    }

    if (buildableId == "track_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(180, 205, 210)
            : byDifficulty(90, 100, 105);
    }

    if (buildableId == "iceball_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(270, 305, 310)
            : byDifficulty(180, 205, 210);
    }

    if (buildableId == "cannon_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(380, 430, 435)
            : byDifficulty(180, 205, 210);
    }

    if (buildableId == "boomerang_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(270, 305, 310)
            : byDifficulty(135, 155, 155);
    }

    if (buildableId == "super_tower") {
        return upgradePathIndex == 0
            ? byDifficulty(2160, 2450, 2495)
            : byDifficulty(4050, 4590, 4680);
    }

    return 999999;
}

const std::array<DifficultyOption, 3>& DifficultyModel::GetDifficultyOptions() {
    return kDifficultyOptions;
}
