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
            case DifficultyType::Easy:   return 125;
            case DifficultyType::Normal: return 140;
            case DifficultyType::Hard:   return 160;
        }
    }

    if (buildableId == "cannon_tower" || buildableId == "bomb_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 235;
            case DifficultyType::Normal: return 260;
            case DifficultyType::Hard:   return 300;
        }
    }

    if (buildableId == "glue_trap") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 170;
            case DifficultyType::Normal: return 190;
            case DifficultyType::Hard:   return 220;
        }
    }

    if (buildableId == "super_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 380;
            case DifficultyType::Normal: return 420;
            case DifficultyType::Hard:   return 500;
        }
    }

    if (buildableId == "iceball_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 160;
            case DifficultyType::Normal: return 180;
            case DifficultyType::Hard:   return 210;
        }
    }

    if (buildableId == "spike_trap") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 70;
            case DifficultyType::Normal: return 80;
            case DifficultyType::Hard:   return 95;
        }
    }

    if (buildableId == "boomerang_tower") {
        switch (difficulty) {
            case DifficultyType::Easy:   return 200;
            case DifficultyType::Normal: return 220;
            case DifficultyType::Hard:   return 260;
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
    if (buildableId == "dart_tower" && upgradePathIndex == 0) {
        if (nextTier == 1) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 120;
                case DifficultyType::Normal: return 140;
                case DifficultyType::Hard:   return 170;
            }
        }
        if (nextTier == 2) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 240;
                case DifficultyType::Normal: return 280;
                case DifficultyType::Hard:   return 330;
            }
        }
    }

    return 999999;
}

const std::array<DifficultyOption, 3>& DifficultyModel::GetDifficultyOptions() {
    return kDifficultyOptions;
}
