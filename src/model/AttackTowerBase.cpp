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

bool AttackTowerBase::CanApplyUpgrade(int pathIndex) const {
    if (pathIndex < 0 || pathIndex >= static_cast<int>(m_UpgradeTiers.size())) {
        return false;
    }

    return m_UpgradeTiers[static_cast<std::size_t>(pathIndex)] < 1;
}

void AttackTowerBase::MarkUpgradeApplied(int pathIndex) {
    if (pathIndex < 0 || pathIndex >= static_cast<int>(m_UpgradeTiers.size())) {
        return;
    }

    m_UpgradeTiers[static_cast<std::size_t>(pathIndex)] = 1;
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
    return enemy && enemy->CanBeTargeted() && enemy->CanReceiveDamage(GetDamageRule());
}

const EnemyModel::DamageRule& AttackTowerBase::GetDamageRule() const {
    return m_CanPopFrozen
        ? EnemyModel::FrozenCapableDamageRule()
        : EnemyModel::BasicDamageRule();
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
