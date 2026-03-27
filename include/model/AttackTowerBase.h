#ifndef ATTACK_TOWER_BASE_H
#define ATTACK_TOWER_BASE_H

#include "model/TowerBase.h"
#include "model/EnemyModel.h"
#include "model/ProjectileModel.h"

class AttackTowerBase : public TowerBase {
public:
    explicit AttackTowerBase(const glm::vec2& position);
    ~AttackTowerBase() override = default;

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    float GetAttackRange() const { return m_Range; }
    int GetDamage() const { return m_Damage; }

protected:
    void UpdateCooldown(float deltaTimeMs);
    std::shared_ptr<EnemyModel> FindTarget(
        const std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

    virtual void Attack(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    );

    virtual std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) = 0;

protected:
    float m_Range = 150.0f;
    float m_AttackIntervalMs = 1000.0f;
    float m_CooldownMs = 0.0f;
    int m_Damage = 1;

    // TODO:
    // 新攻擊塔若有特殊攻擊，優先放在：
    // 1. Attack() override
    // 2. 或 CreateProjectile() 建立特殊 Projectile
    //
    // 例如：
    // - IceBallTower -> 建立 IceBallProjectile，命中後凍結
    // - CannonTower  -> 建立 CannonProjectile，命中後範圍爆炸
    //
    // ❌ 不要把特殊攻擊塞進 GameModel
};

#endif