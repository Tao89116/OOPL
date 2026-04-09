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

class GlueTower : public AttackTowerBase {
public:
    explicit GlueTower(const glm::vec2& position);

protected:
    std::shared_ptr<ProjectileModel> CreateProjectile(
        const std::shared_ptr<EnemyModel>& target
    ) override;
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
