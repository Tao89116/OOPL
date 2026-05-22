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

    if (buildableId == "cannon_tower" || buildableId == "bomb_tower") {
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
