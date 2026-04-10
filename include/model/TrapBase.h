#ifndef TRAP_BASE_H
#define TRAP_BASE_H

#include "model/TowerBase.h"
#include "model/EnemyModel.h"
#include "model/ProjectileModel.h"
#include <unordered_set>

class TrapBase : public TowerBase {
public:
    explicit TrapBase(const glm::vec2& position);
    ~TrapBase() override = default;
    bool OnRoundEnded() override;

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

protected:
    virtual void TriggerOn(const std::shared_ptr<EnemyModel>& enemy) = 0;

protected:
    float m_TriggerRadius = 40.0f;
    bool m_Expired = false;
    std::unordered_set<const EnemyModel*> m_TriggeredEnemies;

    // TODO:
    // TriggerOn()：
    // SpikeTrap: 直接傷害
    // GlueTrap : 套 SlowEffect
    // BombTrap : 範圍傷害
};

#endif
