//
// Created by polyunicorn on 2026/3/13.
//

#ifndef TOWER_MODEL_H
#define TOWER_MODEL_H

#include "pch.hpp"

enum class TowerType {
    Dart,
    Track,
    IceBall
};

class TowerModel {
public:
    TowerModel(TowerType type, const glm::vec2& position);

    void UpdateCooldown(float deltaTimeMs);

    bool CanFire() const { return m_CooldownMs <= 0.0f; }
    void ResetCooldown() { m_CooldownMs = m_AttackIntervalMs; }

    TowerType GetType() const { return m_Type; }
    const glm::vec2& GetPosition() const { return m_Position; }
    float GetRange() const { return m_Range; }
    float GetRadius() const { return m_Radius; }
    bool CanPlaceOnPath() const { return m_CanPlaceOnPath; }

    int GetDamage() const { return m_Damage; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }
    const std::string& GetProjectileKey() const { return m_ProjectileKey; }
    int GetCost() const { return m_Cost; }

    static int GetCostByType(TowerType type);
    static std::string GetDisplayName(TowerType type);
    static float GetRadiusByType(TowerType type);
    static bool GetCanPlaceOnPathByType(TowerType type);

private:
    void SetupStatsByType();

private:
    TowerType m_Type;
    glm::vec2 m_Position;

    float m_Range = 130.0f;
    float m_Radius = 28.0f;
    bool m_CanPlaceOnPath = false;

    float m_AttackIntervalMs = 650.0f;
    float m_CooldownMs = 0.0f;

    int m_Damage = 1;
    int m_Cost = 100;

    std::string m_SpriteKey = "tower_basic";
    std::string m_ProjectileKey = "projectile_0";

    // TODO: 之後若有升級系統，可把 range/damage/cooldown 拆為 base + level bonus。
};

#endif