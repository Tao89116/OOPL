#include "model/AttackTowerBase.h"
#include <cmath>

AttackTowerBase::AttackTowerBase(const glm::vec2& position)
    : TowerBase(position) {
}


int AttackTowerBase::GetUpgradeTier(int pathIndex) const {
    if (pathIndex < 0 || pathIndex >= static_cast<int>(m_UpgradeTiers.size())) {
        return 0;
    }

    return m_UpgradeTiers[static_cast<std::size_t>(pathIndex)];
}

std::string AttackTowerBase::GetUpgradeName(int pathIndex) const {
    if (pathIndex < 0 || pathIndex >= static_cast<int>(m_UpgradeNames.size())) {
        return "";
    }

    return m_UpgradeNames[static_cast<std::size_t>(pathIndex)];
}

bool AttackTowerBase::ApplyUpgrade(int pathIndex) {
    if (pathIndex < 0 || pathIndex >= static_cast<int>(m_UpgradeTiers.size())) {
        return false;
    }

    auto& tier = m_UpgradeTiers[static_cast<std::size_t>(pathIndex)];
    if (tier >= 1) {
        return false;
    }

    if (m_Id == "dart_tower") {
        if (pathIndex == 0) {
            m_Pierce += 1;
        } else {
            m_Range += 25.0f;
            m_PreviewRange = m_Range;
        }
    } else if (m_Id == "super_tower") {
        if (pathIndex == 0) {
            m_Range += 100.0f;
            m_PreviewRange = m_Range;
        } else {
            m_Pierce += 1;
            m_CanPopFrozen = true;
        }
    } else {
        return false;
    }

    tier = 1;
    return true;
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
        if (!CanTargetEnemy(enemy)) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), m_Position);
        if (dist <= m_Range) {
            return enemy;
        }
    }

    return nullptr;
}

bool AttackTowerBase::CanTargetEnemy(const std::shared_ptr<EnemyModel>& enemy) const {
    if (!enemy || !enemy->CanBeTargeted()) {
        return false;
    }

    const EnemyModel::DamageOptions damageOptions = GetDamageOptions();
    if (enemy->IsFrozen() && !damageOptions.canPopFrozen) {
        return false;
    }

    return true;
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
