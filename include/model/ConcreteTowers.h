#ifndef CONCRETE_TOWERS_H
#define CONCRETE_TOWERS_H

#include "model/AttackTowerBase.h"
#include "model/TrapBase.h"

class DartTower : public AttackTowerBase {
public:
    explicit DartTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
};

class TrackTower : public AttackTowerBase {
public:
    explicit TrackTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
};

class IceBallTower : public AttackTowerBase {
public:
    explicit IceBallTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
};

class CannonTower : public AttackTowerBase {
public:
    explicit CannonTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
};

class GlueTrap : public TrapBase {//被緩速之後切成"已緩速"狀態
public:
    explicit GlueTrap(const glm::vec2& position);

protected:
    void TriggerOn(const std::shared_ptr<EnemyModel>& enemy) override;

private:
    int m_RemainingCharges = 20;
    int m_Damage = 0;
    float m_SlowMultiplier = 0.6f;
    float m_SlowDurationMs = 3200.0f;
};

class SuperTower : public AttackTowerBase {
public:
    explicit SuperTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
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
