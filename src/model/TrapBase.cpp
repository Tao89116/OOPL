//
// Created by polyunicorn on 2026/3/26.
//

#include "model/TrapBase.h"

TrapBase::TrapBase(const glm::vec2& position)
    : TowerBase(position) {
}

void TrapBase::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    (void)deltaTimeMs;
    (void)projectiles;

    if (m_Expired) {
        return;
    }

    for (const auto& enemy : enemies) {
        if (!CanTriggerOn(enemy)) {
            continue;
        }

        TriggerOn(enemy);

        if (m_Expired) {
            return;
        }
    }
}

bool TrapBase::CanTriggerOn(const std::shared_ptr<EnemyModel>& enemy) const {
    if (!enemy || !enemy->CanBeTargeted()) {
        return false;
    }

    const glm::vec2 delta = enemy->GetPosition() - m_Position;
    const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    return distance <= m_TriggerRadius;
}