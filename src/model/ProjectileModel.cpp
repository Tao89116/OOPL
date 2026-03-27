#include "model/ProjectileModel.h"

#include <cmath>

ProjectileModel::ProjectileModel(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target
)
    : m_Position(startPos),
      m_Damage(damage),
      m_SpriteKey(spriteKey),
      m_Target(target) {
}

void ProjectileModel::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    auto target = m_Target.lock();
    if (!target || !target->CanBeTargeted()) {
        OnMissOrInvalidTarget();
        return;
    }

    const glm::vec2 targetPos = target->GetPosition();
    const glm::vec2 direction = targetPos - m_Position;
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 8.0f) {
        OnHit(target, enemies);
        m_Active = false;
        return;
    }

    const glm::vec2 normalized = direction / distance;
    m_Position += normalized * m_Speed * deltaTimeMs;
}

void ProjectileModel::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>&
) {
    if (!target) {
        return;
    }

    target->TakeDamage(m_Damage);
}

void ProjectileModel::OnMissOrInvalidTarget() {
    m_Active = false;
}

// ===================== IceBallProjectile =====================

IceBallProjectile::IceBallProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target
)
    : ProjectileModel(startPos, damage, spriteKey, target) {
}

void IceBallProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    ProjectileModel::OnHit(target, enemies);

    // TODO:
    // 等你有 EnemyModel 狀態系統後，把凍結效果放這裡。
    // 例如：
    // target->AddStatusEffect(std::make_shared<FreezeEffect>(m_FreezeDurationMs));
    (void)m_FreezeDurationMs;
}

// ===================== CannonProjectile =====================

CannonProjectile::CannonProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target
)
    : ProjectileModel(startPos, damage, spriteKey, target) {
}

void CannonProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!target) {
        return;
    }

    const glm::vec2 center = target->GetPosition();

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), center);
        if (dist <= m_ExplosionRadius) {
            enemy->TakeDamage(m_Damage);
        }
    }
}