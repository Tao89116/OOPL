#include "model/AttackTowerBase.h"
#include <cmath>

AttackTowerBase::AttackTowerBase(const glm::vec2& position)
    : TowerBase(position) {
}

void AttackTowerBase::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    UpdateCooldown(deltaTimeMs);

    auto target = FindTarget(enemies);
    if (!target) {
        return;
    }

    const glm::vec2 direction = target->GetPosition() - m_Position;
    if (glm::length(direction) > 0.0001f) {
        SetRotation(std::atan2(direction.y, direction.x));
    }

    if (m_CooldownMs <= 0.0f) {
        Attack(target, projectiles);
        m_CooldownMs = m_AttackIntervalMs;
    }
}

void AttackTowerBase::UpdateCooldown(float deltaTimeMs) {
    if (m_CooldownMs > 0.0f) {
        m_CooldownMs -= deltaTimeMs;
    }
}

std::shared_ptr<EnemyModel> AttackTowerBase::FindTarget(
    const std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->IsAlive()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), m_Position);
        if (dist <= m_Range) {
            return enemy;
        }
    }

    return nullptr;
}

void AttackTowerBase::Attack(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    auto projectile = CreateProjectile(target);
    if (projectile) {
        projectiles.push_back(projectile);
    }
}
