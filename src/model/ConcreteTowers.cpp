//
// Created by polyunicorn on 2026/3/26.
//

#include "model/ConcreteTowers.h"

DartTower::DartTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_DisplayName = "Dart Tower";
    m_SpriteKey = "tower_dart";
    m_PreviewSpriteKey = "tower_dart";
    m_Cost = 100;
    m_FootprintRadius = 26.0f;
    m_CanPlaceOnPath = false;
    m_ShowRangePreview = true;

    m_Range = 145.0f;
    m_AttackIntervalMs = 650.0f;
    m_Damage = 1;
    m_ProjectileKey = "projectile_0";
}

TrackTower::TrackTower(const glm::vec2& position)
    : AttackTowerBase(position) {
    m_DisplayName = "Track Tower";
    m_SpriteKey = "tower_track";
    m_PreviewSpriteKey = "tower_track";
    m_Cost = 140;
    m_FootprintRadius = 24.0f;
    m_CanPlaceOnPath = false;
    m_ShowRangePreview = true;

    m_Range = 125.0f;
    m_AttackIntervalMs = 380.0f;
    m_Damage = 1;
    m_ProjectileKey = "projectile_1";
}

IceBallTower::IceBallTower(const glm::vec2& position) //check redundancy 規則實作應該放在concrete tower 還是registry
    : AttackTowerBase(position) {
    m_DisplayName = "IceBall Tower";
    m_SpriteKey = "tower_ice";
    m_PreviewSpriteKey = "tower_ice";
    m_Cost = 180;
    m_FootprintRadius = 30.0f;
    m_CanPlaceOnPath = false;
    m_ShowRangePreview = true;

    m_Range = 165.0f;
    m_AttackIntervalMs = 900.0f;
    m_Damage = 2;
    m_ProjectileKey = "projectile_2";
}

SpikeTrap::SpikeTrap(const glm::vec2& position)
    : TrapBase(position) {
    m_DisplayName = "Spike Trap";
    m_SpriteKey = "tower_basic";
    m_PreviewSpriteKey = "tower_basic";
    m_Cost = 80;
    m_FootprintRadius = 18.0f;
    m_CanPlaceOnPath = true;
    m_ShowRangePreview = false;

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
        m_Expired = true;
    }
}