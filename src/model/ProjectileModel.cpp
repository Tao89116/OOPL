#include "model/ProjectileModel.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>

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

    target->TakeDamage(m_Damage);
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
    float speed
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr),
      m_MaxDistance(maxDistance) {
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
            enemy->TakeDamage(m_Damage);
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
    float freezeDurationMs
)
    : ProjectileModel(centerPos, damage, spriteKey, nullptr),
      m_Center(centerPos),
      m_MaxRadius(maxRadius),
      m_ExpandDurationMs(expandDurationMs),
      m_FreezeDurationMs(freezeDurationMs) {
    m_Position = centerPos;
    m_RenderScale = 0.0f;
}

void ExpandingAoEProjectile::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    m_ElapsedMs += deltaTimeMs;
    const float t = std::clamp(m_ElapsedMs / std::max(m_ExpandDurationMs, 1.0f), 0.0f, 1.0f);
    m_CurrentRadius = m_MaxRadius * t;
    m_RenderScale = std::max(t, 0.01f);

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        const EnemyModel* key = enemy.get();
        if (m_AffectedEnemies.find(key) != m_AffectedEnemies.end()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), m_Center);
        if (dist <= m_CurrentRadius) {
            enemy->TakeDamage(m_Damage);
            enemy->ApplyFreeze(m_FreezeDurationMs);
            m_AffectedEnemies.insert(key);
        }
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
    float maxRange,
    float lifetimeMs,
    float arcHeight,
    int maxPierce
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr),
      m_Origin(startPos),
      m_MaxRange(maxRange),
      m_LifetimeMs(lifetimeMs),
      m_ArcHeight(arcHeight),
      m_MaxPierce(maxPierce) {
    const float len = glm::length(direction);
    if (len > 0.0001f) {
        m_Direction = direction / len;
    }
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
    const float outward = (t <= 0.5f) ? (t * 2.0f) : ((1.0f - t) * 2.0f);
    const float sideOffset = m_ArcHeight * std::sin(glm::pi<float>() * t);

    m_Position = m_Origin + m_Direction * (outward * m_MaxRange) + m_Perpendicular * sideOffset;

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
            enemy->TakeDamage(m_Damage);
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
    const std::shared_ptr<EnemyModel>& target
)
    : ProjectileModel(startPos, damage, spriteKey, target) {
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

// ===================== GlueProjectile =====================

GlueProjectile::GlueProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target
)
    : ProjectileModel(startPos, damage, spriteKey, target) {
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
