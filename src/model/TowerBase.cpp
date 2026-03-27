//
// Created by polyunicorn on 2026/3/26.
//

#include "model/TowerBase.h"

TowerBase::TowerBase(const glm::vec2& position)
    : m_Position(position) {
}

bool TowerBase::CanUpgrade(int upgradePathIndex) const {
    if (upgradePathIndex < 0 || upgradePathIndex >= 3) {
        return false;
    }
    return false;
}

void TowerBase::Upgrade(int upgradePathIndex) {
    (void)upgradePathIndex;
}

int TowerBase::GetUpgradeTier(int upgradePathIndex) const {
    if (upgradePathIndex < 0 || upgradePathIndex >= 3) {
        return 0;
    }
    return m_UpgradeTiers[upgradePathIndex];
}