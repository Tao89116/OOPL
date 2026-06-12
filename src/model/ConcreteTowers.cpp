#include "model/ConcreteTowers.h"
#include "model/StatusEffects/SlowEffect.h"
#include "model/ProjectileModel.h"
#include <algorithm>
#include <memory>
#include <array>
#include <cmath>

DartTower::DartTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "dart_tower";
    m_DisplayName = "Dart Tower";
    m_SpeedText = "Medium";
    m_Description = "Shoots a single dart.";

    m_SpriteKey = "tower_dart";
    m_PreviewSpriteKey = "tower_dart";

    m_Cost = 100;
    m_FootprintRadius = 26.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 180.0f;

    m_Range = 180.0f;
    m_AttackIntervalMs = 825.0f;
    m_Damage = 1;
    m_UpgradeNames = {"Piercing Darts", "Long Range Darts"};
    m_UpgradeIconKeys = {"upgrade_dart_0", "upgrade_dart_1"};
}

bool DartTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_Pierce += 1;
    } else if (pathIndex == 1) {
        m_Range += 25.0f;
        m_PreviewRange = m_Range;
    } else {
        return false;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

std::shared_ptr<ProjectileModel> DartTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    const glm::vec2 toEnemy = target->GetPosition() - m_Position;
    const float len = glm::length(toEnemy);
    const glm::vec2 direction = (len > 0.0001f) ? toEnemy / len : glm::vec2(1.0f, 0.0f);

    return std::make_shared<DirectionalProjectile>(
        m_Position,
        m_Damage,
        "projectile_0",
        direction,
        m_Range,
        2.0f,
        24.0f,
        GetDamageRule(),
        m_Pierce
    );
}

TrackTower::TrackTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "track_tower";
    m_DisplayName = "Track Tower";
    m_SpeedText = "Slow";
    m_Description = "Fires sharp tacks in eight directions.";

    m_SpriteKey = "tower_track";
    m_PreviewSpriteKey = "tower_track";

    m_Cost = 140;
    m_FootprintRadius = 24.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 125.0f;

    m_Range = 125.0f;
    m_AttackIntervalMs = 1375.0f;
    m_Damage = 1;
    m_UpgradeNames = {"Faster Shooting", "Extra Range Tacks"};
    m_UpgradeIconKeys = {"upgrade_tack_0", "upgrade_tack_1"};
}

bool TrackTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_AttackIntervalMs = std::max(50.0f, m_AttackIntervalMs - 15.0f);
    } else {
        m_Range += 10.0f;
        m_PreviewRange = m_Range;
        m_TackHitRadius *= 1.3f;
        m_TackRenderScale *= 1.3f;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

std::shared_ptr<ProjectileModel> TrackTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    (void)target;
    return std::make_shared<DirectionalProjectile>(
        m_Position,
        m_Damage,
        "projectile_1",
        glm::vec2(1.0f, 0.0f),
        125.0f,
        0.9f,
        m_TackHitRadius,
        GetDamageRule(),
        1,
        m_TackRenderScale
    );
}

void TrackTower::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    UpdateCooldown(deltaTimeMs);

    bool hasEnemyInRange = false;
    for (const auto& enemy : enemies) {
        if (!CanTargetEnemy(enemy)) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_Range) {
            hasEnemyInRange = true;
            break;
        }
    }

    if (hasEnemyInRange && m_CooldownMs <= 0.0f) {
        Attack(nullptr, projectiles);
        m_CooldownMs = m_AttackIntervalMs;
    }
}

void TrackTower::Attack(
    const std::shared_ptr<EnemyModel>& target,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    (void)target;
    static const std::array<glm::vec2, 8> kDirections = {{
        {0.0f, 1.0f},
        {0.7071f, 0.7071f},
        {1.0f, 0.0f},
        {0.7071f, -0.7071f},
        {0.0f, -1.0f},
        {-0.7071f, -0.7071f},
        {-1.0f, 0.0f},
        {-0.7071f, 0.7071f}
    }};

    for (const auto& direction : kDirections) {
        projectiles.push_back(std::make_shared<DirectionalProjectile>(
            m_Position,
            m_Damage,
            "projectile_1",
            direction,
            m_Range,
            0.9f,
            m_TackHitRadius,
            GetDamageRule(),
            1,
            m_TackRenderScale
        ));
    }
}

IceBallTower::IceBallTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "iceball_tower";
    m_DisplayName = "Ice Ball Tower";
    m_SpeedText = "Very Slow";
    m_Description = "Freezes nearby bloons with icy blasts.";

    m_SpriteKey = "tower_ice";
    m_PreviewSpriteKey = "tower_ice";

    m_Cost = 180;
    m_FootprintRadius = 30.0f;
    m_CanPlaceOnPath = false;

    m_Range = 110.0f;

    m_ShowRangePreview = true;
    m_PreviewRange = m_Range;
    m_AttackIntervalMs = 2325.0f;
    m_Damage = 0;
    m_UpgradeNames = {"Long Freeze Time", "Wide Freeze Radius"};
    m_UpgradeIconKeys = {"upgrade_ice_0", "upgrade_ice_1"};
}

bool IceBallTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_FreezeDurationMs += 20.0f;
    } else {
        m_Range += 15.0f;
        m_PreviewRange = m_Range;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

std::shared_ptr<ProjectileModel> IceBallTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    (void)target;

    return std::make_shared<ExpandingAoEProjectile>(
        m_Position,
        m_Damage,
        "projectile_2",
        m_Range,
        380.0f,
        m_FreezeDurationMs,
        GetDamageRule(),
        (m_Range > 110.0f) ? 1.2f : 1.0f
    );
}

void IceBallTower::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    UpdateCooldown(deltaTimeMs);

    bool hasEnemyInRange = false;
    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->CanReceiveFreeze()) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_Range) {
            hasEnemyInRange = true;
            break;
        }
    }

    if (!hasEnemyInRange || m_CooldownMs > 0.0f) {
        return;
    }

    auto effect = std::make_shared<ExpandingAoEProjectile>(
        m_Position,
        m_Damage,
        "projectile_2",
        m_Range,
        380.0f,
        m_FreezeDurationMs,
        GetDamageRule(),
        (m_Range > 110.0f) ? 1.2f : 1.0f
    );
    if (effect) {
        projectiles.push_back(effect);
    }

    m_CooldownMs = m_AttackIntervalMs;
}

CannonTower::CannonTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "cannon_tower";
    m_DisplayName = "Cannon Tower";
    m_SpeedText = "Slow";
    m_Description = "Launches bombs that explode on impact.";

    m_SpriteKey = "tower_cannon";
    m_PreviewSpriteKey = "tower_cannon";

    m_Cost = 260;
    m_FootprintRadius = 32.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 216.0f;

    m_Range = 216.0f;
    m_AttackIntervalMs = 1200.0f;
    m_Damage = 1;
    m_CanPopFrozen = true;
    m_UpgradeNames = {"Bigger Bombs", "Extra Range Bombs"};
    m_UpgradeIconKeys = {"upgrade_bomb_0", "upgrade_bomb_1"};
}

bool CannonTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_ExplosionRadius *= 1.5f;
        m_ProjectileScale *= 1.5f;
    } else {
        m_Range += 20.0f;
        m_PreviewRange = m_Range;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

void CannonTower::Update(
    float deltaTimeMs,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>& projectiles
) {
    UpdateCooldown(deltaTimeMs);

    std::shared_ptr<EnemyModel> target = nullptr;
    for (const auto& enemy : enemies) {
        // Black bloons are immune to cannon explosions, so ignore them while
        // choosing an aim target instead of letting them block valid targets.
        if (!CanTargetEnemy(enemy) || enemy->GetType() == EnemyType::Black) {
            continue;
        }

        if (glm::distance(enemy->GetPosition(), m_Position) <= m_Range) {
            target = enemy;
            break;
        }
    }

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

const EnemyModel::DamageRule& CannonTower::GetDamageRule() const {
    return EnemyModel::ExplosiveDamageRule();
}

std::shared_ptr<ProjectileModel> CannonTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    return std::make_shared<CannonProjectile>(
        m_Position,
        m_Damage,
        "projectile_3",
        target,
        GetDamageRule(),
        m_ExplosionRadius,
        m_ProjectileScale
    );
}

SuperTower::SuperTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "super_tower";
    m_DisplayName = "Super Tower";
    m_SpeedText = "Very Fast";
    m_Description = "Rapidly fires powerful projectiles.";

    m_SpriteKey = "tower_super";
    m_PreviewSpriteKey = "tower_super";

    m_Cost = 420;
    m_FootprintRadius = 28.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 252.0f;

    m_Range = 252.0f;
    m_AttackIntervalMs = 100.0f;
    m_Damage = 1;
    m_UpgradeNames = {"Epic Range", "Laser Vision"};
    m_UpgradeIconKeys = {"upgrade_super_0", "upgrade_super_1"};
}

bool SuperTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_Range += 100.0f;
        m_PreviewRange = m_Range;
    } else if (pathIndex == 1) {
        m_Pierce += 1;
        m_CanPopFrozen = true;
        m_ProjectileSpriteKey = "projectile_laser";
    } else {
        return false;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

std::shared_ptr<ProjectileModel> SuperTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    const glm::vec2 toEnemy = target->GetPosition() - m_Position;
    const float len = glm::length(toEnemy);
    const glm::vec2 direction = (len > 0.0001f) ? toEnemy / len : glm::vec2(1.0f, 0.0f);

    return std::make_shared<DirectionalProjectile>(
        m_Position,
        m_Damage,
        m_ProjectileSpriteKey,
        direction,
        m_Range,
        2.0f,
        24.0f,
        GetDamageRule(),
        m_Pierce
    );
}

BoomerangTower::BoomerangTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "boomerang_tower";
    m_DisplayName = "Boomerang Tower";
    m_SpeedText = "Slow";
    m_Description = "Throws boomerangs through multiple bloons.";

    m_SpriteKey = "tower_boom";
    m_PreviewSpriteKey = "tower_boom";

    m_Cost = 220;
    m_FootprintRadius = 28.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 234.0f;

    m_Range = 234.0f;
    m_AttackIntervalMs = 1250.0f;
    m_Damage = 1;
    m_UpgradeNames = {"Multi Target", "Sonic Boom"};
    m_UpgradeIconKeys = {"upgrade_boomerang_0", "upgrade_boomerang_1"};
}

bool BoomerangTower::ApplyUpgrade(int pathIndex) {
    if (!CanApplyUpgrade(pathIndex)) {
        return false;
    }

    if (pathIndex == 0) {
        m_BoomerangPierce += 3;
    } else {
        m_CanPopFrozen = true;
    }

    MarkUpgradeApplied(pathIndex);
    return true;
}

std::shared_ptr<ProjectileModel> BoomerangTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    const glm::vec2 toEnemy = target->GetPosition() - m_Position;
    const float len = glm::length(toEnemy);
    const glm::vec2 direction = (len > 0.0001f) ? toEnemy / len : glm::vec2(1.0f, 0.0f);
    const float diameter = std::max(len, 1.0f);

    return std::make_shared<BoomerangProjectile>(
        m_Position,
        m_Damage,
        "projectile_4",
        direction,
        diameter,
        300.0f,
        m_BoomerangPierce,
        GetDamageRule()
    );
}

SpikeTrap::SpikeTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_Id = "spike_trap";
    m_DisplayName = "Spike Trap";
    m_SpeedText = "On Contact";
    m_Description = "Damages bloons that pass over the spikes.";

    m_SpriteKey = "tower_spikes";
    m_PreviewSpriteKey = "tower_spikes";

    m_Cost = 80;
    m_FootprintRadius = 18.0f;
    m_CanPlaceOnPath = true;

    m_ShowRangePreview = true;
    m_PreviewRange = 25.0f;
    //調整氣球的hitbox
    m_TriggerRadius = 25.0f;
    m_RemainingCharges = 8;
    m_Damage = 1;
}

void SpikeTrap::TriggerOn(const std::shared_ptr<EnemyModel>& enemy) {
    if (!enemy) {
        return;
    }

    enemy->TakeDamage(m_Damage);
    --m_RemainingCharges;

    if (m_RemainingCharges <= 0) {
        m_ShouldRemove = true;
    }
}

GlueTrap::GlueTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_Id = "glue_trap";
    m_DisplayName = "Glue Trap";
    m_SpeedText = "On Contact";
    m_Description = "Slows bloons that step into the glue.";

    m_SpriteKey = "tower_glues";
    m_PreviewSpriteKey = "tower_glues";

    m_Cost = 30;
    m_FootprintRadius = 18.0f;
    m_CanPlaceOnPath = true;

    m_ShowRangePreview = true;
    m_PreviewRange = 25.0f;
    //調整氣球的hitbox
    m_TriggerRadius = 25.0f;
    m_RemainingCharges = 20;
    m_Damage = 0;
}

void GlueTrap::TriggerOn(const std::shared_ptr<EnemyModel>& enemy) {
    if (!enemy) {
        return;
    }

    enemy->TakeDamage(m_Damage);
    if (enemy->CanBeTargeted()) {
        enemy->AddStatusEffect(std::make_unique<SlowEffect>(m_SlowMultiplier, m_SlowDurationMs));
    }
    AddHitEffectEvent({"", "splat", enemy->GetPosition(), 1.0f, 0.0f});
    --m_RemainingCharges;

    if (m_RemainingCharges <= 0) {
        m_ShouldRemove = true;
    }

}
