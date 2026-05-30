#ifndef ATTACK_TOWER_BASE_H
#define ATTACK_TOWER_BASE_H

#include "model/TowerBase.h"
#include "model/EnemyModel.h"
#include "model/ProjectileModel.h"
#include <array>

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

    bool IsUpgradeable() const override { return true; }
    int GetUpgradeTier(int pathIndex) const override;
    std::string GetUpgradeName(int pathIndex) const override;
    bool ApplyUpgrade(int pathIndex) override = 0;

protected:
    bool CanApplyUpgrade(int pathIndex) const;
    void MarkUpgradeApplied(int pathIndex);

protected:
    void UpdateCooldown(float deltaTimeMs);
    std::shared_ptr<EnemyModel> FindTarget(
        const std::vector<std::shared_ptr<EnemyModel>>& enemies
    );
    bool CanTargetEnemy(const std::shared_ptr<EnemyModel>& enemy) const;
    virtual EnemyModel::DamageOptions GetDamageOptions() const { return {m_CanPopFrozen}; }

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
    int m_Pierce = 1;
    bool m_CanPopFrozen = false;
    std::array<int, 2> m_UpgradeTiers = {0, 0};
    std::array<std::string, 2> m_UpgradeNames = {"", ""};

    // TODO:
    // 新攻擊塔若有特殊攻擊放在：
    // 1. Attack() override
    // 2. 或 CreateProjectile() 建立特殊 Projectile
    // 預計:
    // IceBallTower -> 建立 IceBallProjectile，命中後凍結
    // CannonTower  -> 建立 CannonProjectile，命中後範圍爆炸
};

#endif