//
// Created by polyunicorn on 2026/3/26.
//

#include "model/DifficultyModel.h"

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

    if (buildableId == "IceBall_tower") {
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

    return 999999;
}

int DifficultyModel::GetUpgradeCost(
    DifficultyType difficulty,
    const std::string& buildableId,
    int upgradePathIndex,
    int nextTier
) {
    // 目前先保留接口，之後真的做升級再細化
    // 這裡先示範 dart tower 的 path 0 / tier 1, 2
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

    if (buildableId == "track_tower" && upgradePathIndex == 0) {
        if (nextTier == 1) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 150;
                case DifficultyType::Normal: return 170;
                case DifficultyType::Hard:   return 200;
            }
        }

        if (nextTier == 2) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 290;
                case DifficultyType::Normal: return 330;
                case DifficultyType::Hard:   return 390;
            }
        }
    }

    if (buildableId == "IceBall_tower" && upgradePathIndex == 0) {
        if (nextTier == 1) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 210;
                case DifficultyType::Normal: return 240;
                case DifficultyType::Hard:   return 285;
            }
        }

        if (nextTier == 2) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 380;
                case DifficultyType::Normal: return 430;
                case DifficultyType::Hard:   return 510;
            }
        }
    }

    if (buildableId == "spike_trap" && upgradePathIndex == 0) {
        if (nextTier == 1) {
            switch (difficulty) {
                case DifficultyType::Easy:   return 90;
                case DifficultyType::Normal: return 105;
                case DifficultyType::Hard:   return 125;
            }
        }
    }

    return 999999;
}