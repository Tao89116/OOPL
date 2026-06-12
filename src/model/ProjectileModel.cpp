#include "model/ProjectileModel.h"
#include "model/StatusEffects/FreezeEffect.h"
#include "model/StatusEffects/SlowEffect.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <utility>

ProjectileModel::ProjectileModel(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageRule& damageRule,
    int maxPierce,
    float renderScale
)
    : m_Position(startPos),
      m_Damage(damage),
      m_SpriteKey(spriteKey),
      m_Target(target),
      m_DamageRule(&damageRule),
      m_RenderScale(renderScale),
      m_MaxPierce(std::max(1, maxPierce)) {
}

void ProjectileModel::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!m_Active) {
        return;
    }

    if (m_IsPiercing) {
        UpdatePiercingFlight(deltaTimeMs, enemies);
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
        m_HitEnemies.insert(target.get());

        if (static_cast<int>(m_HitEnemies.size()) >= m_MaxPierce) {
            m_Active = false;
            return;
        }

        m_IsPiercing = true;
        m_Target.reset();
        UpdatePiercingFlight(deltaTimeMs, enemies);
        return;
    }

    const glm::vec2 normalized = direction / distance;
    m_PierceDirection = normalized;
    m_Rotation = std::atan2(normalized.y, normalized.x);

    const float step = m_Speed * deltaTimeMs;
    m_Position += normalized * step;
    m_PierceTraveledDistance += step;
}

std::vector<HitEffectEvent> ProjectileModel::ConsumeHitEffectEvents() {
    std::vector<HitEffectEvent> events = std::move(m_HitEffectEvents);
    m_HitEffectEvents.clear();
    return events;
}

void ProjectileModel::AddHitEffectEvent(HitEffectEvent event) {
    m_HitEffectEvents.push_back(std::move(event));
}


void ProjectileModel::UpdatePiercingFlight(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    const float step = m_Speed * deltaTimeMs;
    m_Position += m_PierceDirection * step;
    m_PierceTraveledDistance += step;
    m_Rotation = std::atan2(m_PierceDirection.y, m_PierceDirection.x);

    const float hitRadius = 12.0f * std::max(m_RenderScale, 1.0f);
    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted() ||
            m_HitEnemies.find(enemy.get()) != m_HitEnemies.end()) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= hitRadius) {
            OnHit(enemy, enemies);
            m_HitEnemies.insert(enemy.get());
            if (static_cast<int>(m_HitEnemies.size()) >= m_MaxPierce) {
                m_Active = false;
                return;
            }
        }
    }

    if (m_PierceTraveledDistance >= m_MaxPierceTravelDistance) {
        m_Active = false;
    }
}

void ProjectileModel::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>&
) {
    if (!target) {
        return;
    }

    target->TakeDamage(m_Damage, *m_DamageRule);
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
    const EnemyModel::DamageRule& damageRule,
    int maxPierce,
    float renderScale
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr, damageRule, maxPierce, renderScale),
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
        if (!enemy || !enemy->CanBeTargeted() ||
            m_HitEnemies.find(enemy.get()) != m_HitEnemies.end()) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_HitRadius) {
            enemy->TakeDamage(m_Damage, *m_DamageRule);
            m_HitEnemies.insert(enemy.get());
            if (static_cast<int>(m_HitEnemies.size()) >= m_MaxPierce) {
                m_Active = false;
                return;
            }
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
    const EnemyModel::DamageRule& damageRule,
    float renderScale
)
    : ProjectileModel(centerPos, damage, spriteKey, nullptr, damageRule, 1, renderScale),
      m_Center(centerPos),
      m_MaxRadius(maxRadius),
      m_ExpandDurationMs(expandDurationMs),
      m_FreezeDurationMs(freezeDurationMs),
      m_RenderScaleMultiplier(renderScale) {
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
    m_RenderScale = t * m_RenderScaleMultiplier;

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
                enemy->TakeDamage(m_Damage, *m_DamageRule);
                if (enemy->CanReceiveFreeze()) {
                    enemy->AddStatusEffect(std::make_unique<FreezeEffect>(m_FreezeDurationMs));
                }
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
    const EnemyModel::DamageRule& damageRule
)
    : ProjectileModel(startPos, damage, spriteKey, nullptr, damageRule),
      m_Origin(startPos),
      m_Radius(diameter * 0.5f),
      m_LifetimeMs(lifetimeMs),
      m_MaxPierce(std::max(1, maxPierce)) {
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
            enemy->TakeDamage(m_Damage, *m_DamageRule);
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
    const EnemyModel::DamageRule& damageRule
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageRule) {
}

void IceBallProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    ProjectileModel::OnHit(target, enemies);

    if (target && target->CanReceiveFreeze()) {
        target->AddStatusEffect(std::make_unique<FreezeEffect>(m_FreezeDurationMs));
    }
}

// ===================== CannonProjectile =====================

CannonProjectile::CannonProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageRule& damageRule,
    float explosionRadius,
    float renderScale
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageRule, 1, renderScale),
      m_ExplosionRadius(explosionRadius) {
}

void CannonProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    if (!target) {
        return;
    }

    const glm::vec2 center = target->GetPosition();

    AddHitEffectEvent({
        "tower3_explosion",
        "bombsnd",
        center,
        std::max(m_ExplosionRadius / 40.0f, 0.1f),
        260.0f,
        13,
        20.0f
    });

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanBeTargeted()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), center);
        if (dist <= m_ExplosionRadius) {
            enemy->TakeDamage(m_Damage, *m_DamageRule);
        }
    }
}

// ===================== GlueProjectile =====================

GlueProjectile::GlueProjectile(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target,
    const EnemyModel::DamageRule& damageRule
)
    : ProjectileModel(startPos, damage, spriteKey, target, damageRule) {
}

void GlueProjectile::OnHit(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<EnemyModel>>& enemies
) {
    ProjectileModel::OnHit(target, enemies);

    if (target && target->CanBeTargeted()) {
        target->AddStatusEffect(std::make_unique<SlowEffect>(m_SlowMultiplier, m_SlowDurationMs));
    }

    (void)enemies;
}
