#ifndef PROJECTILE_MODEL_H
#define PROJECTILE_MODEL_H

#include "pch.hpp"
#include "model/EnemyModel.h"
#include "model/HitEffectEmitter.h"
#include <unordered_set>

class ProjectileModel : public HitEffectEmitter {
public:
    ProjectileModel(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule(),
        int maxPierce = 1,
        float renderScale = 1.0f
    );

    virtual ~ProjectileModel() = default;

    virtual void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

    bool IsActive() const { return m_Active; }
    const glm::vec2& GetPosition() const { return m_Position; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }
    virtual float GetRotation() const { return m_Rotation; }
    virtual float GetRenderScale() const { return m_RenderScale; }
    virtual float GetVisualRadius() const { return -1.0f; }
    std::vector<HitEffectEvent> ConsumeHitEffectEvents() override;

protected:
    virtual void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

    virtual void OnMissOrInvalidTarget();
    void AddHitEffectEvent(HitEffectEvent event);
    void UpdatePiercingFlight(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

protected:
    glm::vec2 m_Position;
    float m_Speed = 1.0f;
    int m_Damage = 1;
    bool m_Active = true;
    std::string m_SpriteKey;
    std::weak_ptr<EnemyModel> m_Target;
    const EnemyModel::DamageRule* m_DamageRule;
    float m_Rotation = 0.0f;
    float m_RenderScale = 1.0f;
    int m_MaxPierce = 1;
    bool m_IsPiercing = false;
    glm::vec2 m_PierceDirection = {1.0f, 0.0f};
    float m_PierceTraveledDistance = 0.0f;
    float m_MaxPierceTravelDistance = 900.0f;
    std::unordered_set<const EnemyModel*> m_HitEnemies;
    std::vector<HitEffectEvent> m_HitEffectEvents;
};

class DirectionalProjectile : public ProjectileModel {
public:
    DirectionalProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const glm::vec2& direction,
        float maxDistance,
        float speed = 0.7f,
        float hitRadius = 10.0f,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule(),
        int maxPierce = 1,
        float renderScale = 1.0f
    );

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

protected:
    glm::vec2 m_Direction = {1.0f, 0.0f};
    float m_MaxDistance = 80.0f;
    float m_TraveledDistance = 0.0f;
    float m_HitRadius = 10.0f;
};

class ExpandingAoEProjectile : public ProjectileModel {
public:
    ExpandingAoEProjectile(
        const glm::vec2& centerPos,
        int damage,
        const std::string& spriteKey,
        float maxRadius,
        float expandDurationMs,
        float freezeDurationMs,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule(),
        float renderScale = 1.0f
    );

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;
    float GetVisualRadius() const override { return m_CurrentRadius; }

protected:
    glm::vec2 m_Center;
    float m_MaxRadius = 90.0f;
    float m_CurrentRadius = 0.0f;
    float m_InitialRadius = 0.0f;
    float m_ExpandDurationMs = 350.0f;
    float m_ElapsedMs = 0.0f;
    float m_FreezeDurationMs = 1200.0f;
    float m_RenderScaleMultiplier = 1.0f;
    bool m_HasAppliedEffect = false;
    std::unordered_set<const EnemyModel*> m_AffectedEnemies;
};

class BoomerangProjectile : public ProjectileModel {
public:
    BoomerangProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const glm::vec2& direction,
        float diameter,
        float lifetimeMs,
        int maxPierce,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule()
    );

    void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

private:
    glm::vec2 m_Origin;
    glm::vec2 m_Direction = {1.0f, 0.0f};
    glm::vec2 m_Perpendicular = {0.0f, 1.0f};
    glm::vec2 m_CircleCenter = {0.0f, 0.0f};
    float m_Radius = 65.0f;
    float m_LifetimeMs = 700.0f;
    float m_ElapsedMs = 0.0f;
    float m_HitRadius = 20.0f;
    int m_MaxPierce = 2;
    std::unordered_set<const EnemyModel*> m_HitEnemies;
};

// ===================== IceBall Projectile =====================
class IceBallProjectile : public ProjectileModel {
public:
    IceBallProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule()
    );

protected:
    void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;
private:
    float m_FreezeDurationMs = 1200.0f;
};

// ===================== Cannon Projectile =====================
class CannonProjectile : public ProjectileModel {
public:
    CannonProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule(),
        float explosionRadius = 75.0f,
        float renderScale = 1.0f
    );

protected:
    void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

private:
    float m_ExplosionRadius = 75.0f;
};

// ===================== Glue Projectile =====================
class GlueProjectile : public ProjectileModel {
public:
    GlueProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target,
        const EnemyModel::DamageRule& damageRule = EnemyModel::BasicDamageRule()
    );

protected:
    void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

private:
    float m_SlowMultiplier = 0.6f;
    float m_SlowDurationMs = 1600.0f;
};

#endif
