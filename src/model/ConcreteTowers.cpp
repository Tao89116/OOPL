#include "model/ConcreteTowers.h"
#include "model/ProjectileModel.h"
#include <array>

DartTower::DartTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "dart_tower";
    m_DisplayName = "Dart Tower";

    m_SpriteKey = "tower_dart";
    m_PreviewSpriteKey = "tower_dart";

    m_Cost = 100;
    m_FootprintRadius = 26.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 145.0f;

    m_Range = 145.0f;
    m_AttackIntervalMs = 650.0f;
    m_Damage = 1;
}

std::shared_ptr<ProjectileModel> DartTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    return std::make_shared<ProjectileModel>(
        m_Position,
        m_Damage,
        "projectile_0",
        target
    );
}

TrackTower::TrackTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "track_tower";
    m_DisplayName = "Track Tower";

    m_SpriteKey = "tower_track";
    m_PreviewSpriteKey = "tower_track";

    m_Cost = 140;
    m_FootprintRadius = 24.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 125.0f;

    m_Range = 125.0f;
    m_AttackIntervalMs = 380.0f;
    m_Damage = 1;
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
        72.0f,
        0.9f
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
        if (!enemy || !enemy->IsAlive()) {
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
            72.0f,
            0.9f
        ));
    }
}

IceBallTower::IceBallTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "iceball_tower";
    m_DisplayName = "Ice Ball Tower";

    m_SpriteKey = "tower_ice";
    m_PreviewSpriteKey = "tower_ice";

    m_Cost = 180;
    m_FootprintRadius = 30.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 165.0f;

    m_Range = 170.0f;
    m_AttackIntervalMs = 900.0f;
    m_Damage = 0;
}

std::shared_ptr<ProjectileModel> IceBallTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    (void)target;

    return std::make_shared<ExpandingAoEProjectile>(
        m_Position,
        m_Damage,
        "projectile_2",
        165.0f,
        380.0f,
        1200.0f
    );
}

CannonTower::CannonTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "cannon_tower";
    m_DisplayName = "Cannon Tower";

    m_SpriteKey = "tower_cannon";
    m_PreviewSpriteKey = "tower_cannon";

    m_Cost = 260;
    m_FootprintRadius = 32.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 180.0f;

    m_Range = 175.0f;
    m_AttackIntervalMs = 1200.0f;
    m_Damage = 1;
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
        target
    );
}

SuperTower::SuperTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "super_tower";
    m_DisplayName = "Super Tower";

    m_SpriteKey = "tower_super";
    m_PreviewSpriteKey = "tower_super";

    m_Cost = 420;
    m_FootprintRadius = 28.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 230.0f;

    m_Range = 230.0f;
    m_AttackIntervalMs = 120.0f;
    m_Damage = 1;
}

std::shared_ptr<ProjectileModel> SuperTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    return std::make_shared<ProjectileModel>(
        m_Position,
        m_Damage,
        "projectile_5",
        target
    );
}

BoomerangTower::BoomerangTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "boomerang_tower";
    m_DisplayName = "Boomerang Tower";

    m_SpriteKey = "tower_boom";
    m_PreviewSpriteKey = "tower_boom";

    m_Cost = 220;
    m_FootprintRadius = 28.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 160.0f;

    m_Range = 160.0f;
    m_AttackIntervalMs = 700.0f;
    m_Damage = 1;
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

    return std::make_shared<BoomerangProjectile>(
        m_Position,
        m_Damage,
        "projectile_4",
        direction,
        130.0f,
        760.0f,
        36.0f,
        6
    );
}

SpikeTrap::SpikeTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_Id = "spike_trap";
    m_DisplayName = "Spike Trap";

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

    // TODO:
    // GlueTrap -> enemy->AddStatusEffect(...)
    // BombTrap -> 範圍傷害
}

GlueTrap::GlueTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_Id = "glue_trap";
    m_DisplayName = "Glue Trap";

    m_SpriteKey = "tower_glues";
    m_PreviewSpriteKey = "tower_glues";

    m_Cost = 80;
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
    enemy->ApplySlow(m_SlowMultiplier,m_SlowDurationMs);
    --m_RemainingCharges;

    if (m_RemainingCharges <= 0) {
        m_ShouldRemove = true;
    }

}
