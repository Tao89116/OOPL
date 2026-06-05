#ifndef CONCRETE_TOWERS_H
#define CONCRETE_TOWERS_H

#include "model/AttackTowerBase.h"
#include "model/TrapBase.h"

class DartTower : public AttackTowerBase {
public:
    explicit DartTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
};

class TrackTower : public AttackTowerBase {
public:
    explicit TrackTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    void Attack(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;

private:
    float m_TackHitRadius = 18.0f;
    float m_TackRenderScale = 1.0f;
};

class IceBallTower : public AttackTowerBase {
public:
    explicit IceBallTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;

private:
    float m_FreezeDurationMs = 1250.0f;
};

class CannonTower : public AttackTowerBase {
public:
    explicit CannonTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    const EnemyModel::DamageRule& GetDamageRule() const override;
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;

private:
    float m_ExplosionRadius = 75.0f;
    float m_ProjectileScale = 1.0f;
};

class GlueTrap : public TrapBase {//被緩速之後切成"已緩速"狀態
public:
    explicit GlueTrap(const glm::vec2& position);

protected:
    void TriggerOn(const std::shared_ptr<EnemyModel>& enemy) override;

private:
    int m_RemainingCharges = 20;
    int m_Damage = 0;
    float m_SlowMultiplier = 0.1f;
    float m_SlowDurationMs = 3200.0f;
};

class SuperTower : public AttackTowerBase {
public:
    explicit SuperTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;

private:
    std::string m_ProjectileSpriteKey = "projectile_5";
};

class BoomerangTower : public AttackTowerBase {
public:
    explicit BoomerangTower(const glm::vec2& position);
    bool ApplyUpgrade(int pathIndex) override;

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;

private:
    int m_BoomerangPierce = 2;
};

class SpikeTrap : public TrapBase {
public:
    explicit SpikeTrap(const glm::vec2& position);

protected:
    void TriggerOn(const std::shared_ptr<EnemyModel>& enemy) override;

private:
    int m_RemainingCharges = 8;
    int m_Damage = 1;
};

#endif
