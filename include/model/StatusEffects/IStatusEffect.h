#ifndef I_STATUS_EFFECT_H
#define I_STATUS_EFFECT_H

class EnemyModel;

class IStatusEffect {
public:
    virtual ~IStatusEffect() = default;

    virtual void OnApply(EnemyModel& enemy) = 0;
    virtual void Update(EnemyModel& enemy, float deltaTimeMs) = 0;
    virtual bool IsExpired() const = 0;
    virtual float GetSpeedMultiplier() const = 0;
    virtual bool BlocksDamage() const { return false; }
};

#endif
