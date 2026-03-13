//
// Created by polyunicorn on 2026/3/13.
//

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

    void Update(float deltaTimeMs);

    bool IsActive() const { return m_Active; }
    const glm::vec2& GetPosition() const { return m_Position; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }

private:
    glm::vec2 m_Position;
    float m_Speed = 0.55f;
    int m_Damage = 1;
    bool m_Active = true;
    std::string m_SpriteKey;
    std::weak_ptr<EnemyModel> m_Target;
};

#endif