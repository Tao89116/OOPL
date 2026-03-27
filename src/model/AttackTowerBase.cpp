//
// Created by polyunicorn on 2026/3/26.
//

#include "model/AttackTowerBase.h"

AttackTowerBase::AttackTowerBase(const glm::vec2& position)
    : TowerBase(position) {
}

void AttackTowerBase::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    if (m_CooldownMs > 0.0f) {
        m_CooldownMs -= deltaTimeMs;
    }

    if (m_CooldownMs > 0.0f) {
        return;
    }

    const auto target = SelectTarget(enemies);
    if (!target) {
        return;
    }

    FireAt(target, projectiles);
    m_CooldownMs = m_AttackIntervalMs;
}

std::shared_ptr<EnemyModel> AttackTowerBase::SelectTarget(
    const std::vector<std::shared_ptr<EnemyModel>>& enemies
) const {
    std::shared_ptr<EnemyModel> target = nullptr;
    float nearestDistance = 999999.0f;

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        const glm::vec2 delta = enemy->GetPosition() - m_Position;
        const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance <= m_Range && distance < nearestDistance) {
            nearestDistance = distance;
            target = enemy;
        }
    }

    return target;
}

void AttackTowerBase::FireAt(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    auto projectile = std::make_shared<ProjectileModel>(
        m_Position,
        m_Damage,
        m_ProjectileKey,
        target
    );
    projectiles.push_back(projectile);
}