#ifndef FREEZE_EFFECT_H
#define FREEZE_EFFECT_H

#include "model/StatusEffects/IStatusEffect.h"

#include <algorithm>

class FreezeEffect final : public IStatusEffect {
public:
    explicit FreezeEffect(float durationMs)
        : m_RemainingMs(std::max(0.0f, durationMs)) {}

    void OnApply(EnemyModel& enemy) override;
    void Update(EnemyModel& enemy, float deltaTimeMs) override;
    bool IsExpired() const override { return m_RemainingMs <= 0.0f; }
    float GetSpeedMultiplier() const override { return IsExpired() ? 1.0f : 0.0f; }
    bool BlocksDamage() const override { return !IsExpired(); }

private:
    float m_RemainingMs = 0.0f;
};

#endif
