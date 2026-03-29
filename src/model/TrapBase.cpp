#include "model/TrapBase.h"

TrapBase::TrapBase(const glm::vec2& position)
    : TowerBase(position) {
}

void TrapBase::Update(
    float,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>&
) {
    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->IsAlive()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), m_Position);
        if (dist <= m_TriggerRadius) {
            TriggerOn(enemy);
        }
    }
}
//TODO: 陷阱的hitbox