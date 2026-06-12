#ifndef SLOW_EFFECT_H
#define SLOW_EFFECT_H

#include "model/StatusEffects/IStatusEffect.h"

#include <algorithm>

class SlowEffect final : public IStatusEffect {
public:
    SlowEffect(float speedMultiplier, float durationMs)
        : m_SpeedMultiplier(std::clamp(speedMultiplier, 0.1f, 1.0f)),
          m_RemainingMs(std::max(0.0f, durationMs)) {}

    void OnApply(EnemyModel& enemy) override;
    void Update(EnemyModel& enemy, float deltaTimeMs) override;
    bool IsExpired() const override { return m_RemainingMs <= 0.0f; }
    float GetSpeedMultiplier() const override { return IsExpired() ? 1.0f : m_SpeedMultiplier; }

private:
    float m_SpeedMultiplier = 1.0f;
    float m_RemainingMs = 0.0f;
};

#endif
