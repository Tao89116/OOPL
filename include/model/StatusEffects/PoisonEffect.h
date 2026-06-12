#ifndef POISON_EFFECT_H
#define POISON_EFFECT_H

#include "model/StatusEffects/IStatusEffect.h"

#include <algorithm>

class PoisonEffect final : public IStatusEffect {
public:
    PoisonEffect(int damagePerTick, float durationMs, float tickIntervalMs)
        : m_DamagePerTick(std::max(0, damagePerTick)),
          m_RemainingMs(std::max(0.0f, durationMs)),
          m_TickIntervalMs(std::max(1.0f, tickIntervalMs)) {}

    void OnApply(EnemyModel& enemy) override;
    void Update(EnemyModel& enemy, float deltaTimeMs) override;
    bool IsExpired() const override { return m_RemainingMs <= 0.0f; }
    float GetSpeedMultiplier() const override { return 1.0f; }

private:
    int m_DamagePerTick = 0;
    float m_RemainingMs = 0.0f;
    float m_TickIntervalMs = 1000.0f;
    float m_ElapsedSinceTickMs = 0.0f;
};

#endif
