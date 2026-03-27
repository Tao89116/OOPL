//
// Created by polyunicorn on 2026/3/26.
//

#ifndef ATTACK_TOWER_BASE_H
#define ATTACK_TOWER_BASE_H

#include "model/TowerBase.h"

class AttackTowerBase : public TowerBase {
public:
    explicit AttackTowerBase(const glm::vec2& position);

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    float GetRange() const override { return m_Range; }

protected:
    virtual std::shared_ptr<EnemyModel> SelectTarget(
        const std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) const;

    virtual void FireAt(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    );

protected:
    float m_Range = 120.0f;
    float m_AttackIntervalMs = 650.0f;
    float m_CooldownMs = 0.0f;
    int m_Damage = 1;
    std::string m_ProjectileKey = "projectile_0";
};

#endif