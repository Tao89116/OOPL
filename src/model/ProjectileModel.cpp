#include "model/ProjectileModel.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>

ProjectileModel::ProjectileModel(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageOptions& damageOptions
)
    : m_Position(startPos),
      m_Damage(damage),
      m_SpriteKey(spriteKey),
      m_Target(target),
      m_DamageOptions(damageOptions) {
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
    m_Rotation = std::atan2(normalized.y, normalized.x);
    m_Position += normalized * m_Speed * deltaTimeMs;
}

void ProjectileModel::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>&
) {
    if (!target) {
        return;
    }

    target->TakeDamage(m_Damage, m_DamageOptions);
}

void ProjectileModel::OnMissOrInvalidTarget() {
    m_Active = false;
}

DirectionalProjectile::DirectionalProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const glm::vec2& direction,
    float maxDistance,
    float speed,
    float hitRadius,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr, damageOptions),
      m_MaxDistance(maxDistance),
      m_HitRadius(hitRadius) {
    const float dirLen = glm::length(direction);
    if (dirLen > 0.0001f) {
        m_Direction = direction / dirLen;
    }
    m_Speed = speed;
    m_Rotation = std::atan2(m_Direction.y, m_Direction.x);
}

void DirectionalProjectile::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    const float step = m_Speed * deltaTimeMs;
    m_Position += m_Direction * step;
    m_TraveledDistance += step;

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_HitRadius) {
            enemy->TakeDamage(m_Damage, m_DamageOptions);
            m_Active = false;
            return;
        }
    }

    if (m_TraveledDistance >= m_MaxDistance) {
        m_Active = false;
    }
}

ExpandingAoEProjectile::ExpandingAoEProjectile(
    const glm::vec2& centerPos,
    int damage,
    const std::string& spriteKey,
    float maxRadius,
    float expandDurationMs,
    float freezeDurationMs,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(centerPos, damage, spriteKey, nullptr, damageOptions),
      m_Center(centerPos),
      m_MaxRadius(maxRadius),
      m_ExpandDurationMs(expandDurationMs),
      m_FreezeDurationMs(freezeDurationMs) {
    m_Position = centerPos;
    m_Speed = 0.0f; // 固定在塔中心，不做飛行投射
    m_CurrentRadius = m_InitialRadius;
    m_RenderScale = 0.0f;
}

void ExpandingAoEProjectile::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    // 保持特效錨定在中心點，避免任何「被丟出去」的位移感
    m_Position = m_Center;
    m_ElapsedMs += deltaTimeMs;
    const float t = std::clamp(m_ElapsedMs / std::max(m_ExpandDurationMs, 1.0f), 0.0f, 1.0f);
    m_CurrentRadius = m_InitialRadius + (m_MaxRadius - m_InitialRadius) * t;
    m_RenderScale = t;

    if (!m_HasAppliedEffect) {
        for (const auto& enemy : enemies) {
            if (!enemy || !enemy->CanBeTargeted()) {
                continue;
            }

            const EnemyModel* key = enemy.get();
            if (m_AffectedEnemies.find(key) != m_AffectedEnemies.end()) {
                continue;
            }

            const float dist = glm::distance(enemy->GetPosition(), m_Center);
            if (dist <= m_MaxRadius) {
                enemy->TakeDamage(m_Damage, m_DamageOptions);
                enemy->ApplyFreeze(m_FreezeDurationMs);
                m_AffectedEnemies.insert(key);
            }
        }
        m_HasAppliedEffect = true;
    }

    if (m_ElapsedMs >= m_ExpandDurationMs) {
        m_Active = false;
    }
}

BoomerangProjectile::BoomerangProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const glm::vec2& direction,
    float diameter,
    float lifetimeMs,
    int maxPierce,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr, damageOptions),
      m_Origin(startPos),
      m_Radius(diameter * 0.5f),
      m_LifetimeMs(lifetimeMs),
      m_MaxPierce(std::clamp(maxPierce, 1, 2)) {
    const float len = glm::length(direction);
    if (len > 0.0001f) {
        m_Direction = direction / len;
    }
    m_CircleCenter = m_Origin + m_Direction * m_Radius;
    m_Perpendicular = {-m_Direction.y, m_Direction.x};
}

void BoomerangProjectile::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    const glm::vec2 prevPos = m_Position;

    m_ElapsedMs += deltaTimeMs;
    const float t = std::clamp(m_ElapsedMs / std::max(m_LifetimeMs, 1.0f), 0.0f, 1.0f);
    const float angle = glm::two_pi<float>() * t;

    m_Position = m_CircleCenter
        - m_Direction * (m_Radius * std::cos(angle))
        + m_Perpendicular * (m_Radius * std::sin(angle));

    const glm::vec2 movement = m_Position - prevPos;
    if (glm::length(movement) > 0.0001f) {
        m_Rotation = std::atan2(movement.y, movement.x);
    }

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        const EnemyModel* key = enemy.get();
        if (m_HitEnemies.find(key) != m_HitEnemies.end()) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_HitRadius) {
            enemy->TakeDamage(m_Damage, m_DamageOptions);
            m_HitEnemies.insert(key);
            if (static_cast<int>(m_HitEnemies.size()) >= m_MaxPierce) {
                m_Active = false;
                return;
            }
        }
    }

    if (m_ElapsedMs >= m_LifetimeMs) {
        m_Active = false;
    }
}

// ===================== IceBallProjectile =====================


IceBallProjectile::IceBallProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageOptions) {
}

void IceBallProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    ProjectileModel::OnHit(target, enemies);

    if (target) {
        target->ApplyFreeze(m_FreezeDurationMs);
    }
}

// ===================== CannonProjectile =====================

CannonProjectile::CannonProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageOptions) {
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
            enemy->TakeDamage(m_Damage, m_DamageOptions);
        }
    }
}

// ===================== GlueProjectile =====================

GlueProjectile::GlueProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageOptions& damageOptions
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageOptions) {
}

void GlueProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    ProjectileModel::OnHit(target, enemies);

    if (target) {
        target->ApplySlow(m_SlowMultiplier, m_SlowDurationMs);
    }

    (void)enemies;
}
