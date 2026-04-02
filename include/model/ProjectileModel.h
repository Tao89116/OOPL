#ifndef PROJECTILE_MODEL_H
#define PROJECTILE_MODEL_H

#include "pch.hpp"
#include "model/EnemyModel.h"

class ProjectileModel {
public:
    ProjectileModel(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target
    );

    virtual ~ProjectileModel() = default;

    virtual void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

    bool IsActive() const { return m_Active; }
    const glm::vec2& GetPosition() const { return m_Position; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }

protected:
    virtual void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    );

    virtual void OnMissOrInvalidTarget();

protected:
    glm::vec2 m_Position;
    float m_Speed = 0.55f;
    int m_Damage = 1;
    bool m_Active = true;
    std::string m_SpriteKey;
    std::weak_ptr<EnemyModel> m_Target;
};

// ===================== IceBall Projectile =====================
class IceBallProjectile : public ProjectileModel {
public:
    IceBallProjectile(
        const glm::vec2& startPos,
        int damage,
        const std::string& spriteKey,
        const std::shared_ptr<EnemyModel>& target
    );

protected:
    void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

    // TODO:
    // target->AddStatusEffect(std::make_shared<FreezeEffect>(freezeDurationMs));
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
        const std::shared_ptr<EnemyModel>& target
    );

protected:
    void OnHit(
        const std::shared_ptr<EnemyModel>& target,
        std::vector<std::shared_ptr<EnemyModel>>& enemies
    ) override;

private:
    float m_ExplosionRadius = 75.0f;
};

#endif