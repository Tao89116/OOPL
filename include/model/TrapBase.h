//
// Created by polyunicorn on 2026/3/26.
//

#ifndef TRAP_BASE_H
#define TRAP_BASE_H

#include "model/TowerBase.h"

class TrapBase : public TowerBase {
public:
    explicit TrapBase(const glm::vec2& position);

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) override;

    bool ShouldRemove() const override { return m_Expired; }

protected:
    virtual bool CanTriggerOn(const std::shared_ptr<EnemyModel>& enemy) const;
    virtual void TriggerOn(const std::shared_ptr<EnemyModel>& enemy) = 0;

protected:
    float m_TriggerRadius = 20.0f;
    bool m_Expired = false;
};

#endif