#include "model/ConcreteTowers.h"
#include "model/ProjectileModel.h"

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
    if (!target) {
        return nullptr;
    }

    return std::make_shared<ProjectileModel>(
        m_Position,
        m_Damage,
        "projectile_1",
        target
    );
}

IceBallTower::IceBallTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_Id = "iceball_tower";
    m_DisplayName = "IceBall Tower";

    m_SpriteKey = "tower_ice";
    m_PreviewSpriteKey = "tower_ice";

    m_Cost = 180;
    m_FootprintRadius = 30.0f;
    m_CanPlaceOnPath = false;

    m_ShowRangePreview = true;
    m_PreviewRange = 165.0f;

    m_Range = 165.0f;
    m_AttackIntervalMs = 900.0f;
    m_Damage = 2;
}

std::shared_ptr<ProjectileModel> IceBallTower::CreateProjectile(
    const std::shared_ptr<EnemyModel>& target
) {
    if (!target) {
        return nullptr;
    }

    return std::make_shared<IceBallProjectile>(
        m_Position,
        m_Damage,
        "projectile_2",
        target
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

    m_Range = 180.0f;
    m_AttackIntervalMs = 1200.0f;
    m_Damage = 3;
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

SpikeTrap::SpikeTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_Id = "spike_trap";
    m_DisplayName = "Spike Trap";

    m_SpriteKey = "tower_spikes";
    m_PreviewSpriteKey = "tower_spikes";

    //m_Cost = 80;
    m_FootprintRadius = 18.0f;
    m_CanPlaceOnPath = true;

    m_ShowRangePreview = false;
    m_PreviewRange = 0.0f;

    m_TriggerRadius = 18.0f;
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
    // 新陷阱特殊效果放這裡：
    // - GlueTrap -> enemy->AddStatusEffect(...)
    // - BombTrap -> 範圍傷害
}