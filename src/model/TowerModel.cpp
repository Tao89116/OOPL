//
// Created by polyunicorn on 2026/3/13.
//

#include "model/TowerModel.h"

TowerModel::TowerModel(TowerType type, const glm::vec2& position)
    : m_Type(type), m_Position(position) {
    SetupStatsByType();
}

void TowerModel::SetupStatsByType() {
    switch (m_Type) {
        case TowerType::Dart:
            m_Range = 145.0f;
            m_AttackIntervalMs = 650.0f;
            m_Damage = 1;
            m_Cost = 100;
            m_SpriteKey = "tower_basic";
            m_ProjectileKey = "projectile_0";
            break;
        case TowerType::Rapid:
            m_Range = 125.0f;
            m_AttackIntervalMs = 380.0f;
            m_Damage = 1;
            m_Cost = 140;
            m_SpriteKey = "tower_basic";
            m_ProjectileKey = "projectile_1";
            break;
        case TowerType::Heavy:
            m_Range = 165.0f;
            m_AttackIntervalMs = 900.0f;
            m_Damage = 2;
            m_Cost = 180;
            m_SpriteKey = "tower_basic";
            m_ProjectileKey = "projectile_2";
            break;
    }
}

void TowerModel::UpdateCooldown(float deltaTimeMs) {
    if (m_CooldownMs > 0.0f) {
        m_CooldownMs -= deltaTimeMs;
    }
}

int TowerModel::GetCostByType(TowerType type) {
    switch (type) {
        case TowerType::Dart: return 100;
        case TowerType::Rapid: return 140;
        case TowerType::Heavy: return 180;
    }
    return 100;
}

std::string TowerModel::GetDisplayName(TowerType type) {
    switch (type) {
        case TowerType::Dart: return "Dart Tower";
        case TowerType::Rapid: return "Rapid Tower";
        case TowerType::Heavy: return "Heavy Tower";
    }
    return "Unknown Tower";
}