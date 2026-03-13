//
// Created by polyunicorn on 2026/3/13.
//
#include "model/ProjectileModel.h"

#include <cmath>

ProjectileModel::ProjectileModel(
    const glm::vec2& startPos,
    int damage,
    const std::string& spriteKey,
    const std::shared_ptr<EnemyModel>& target
)
    : m_Position(startPos), m_Damage(damage), m_SpriteKey(spriteKey), m_Target(target) {
}

void ProjectileModel::Update(float deltaTimeMs) {
    if (!m_Active) {
        return;
    }

    auto target = m_Target.lock();
    if (!target || !target->CanBeTargeted()) {
        m_Active = false;
        return;
    }

    const glm::vec2 targetPos = target->GetPosition();
    const glm::vec2 direction = targetPos - m_Position;
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 8.0f) {
        target->TakeDamage(m_Damage);
        m_Active = false;
        return;
    }

    const glm::vec2 normalized = direction / distance;
    m_Position += normalized * m_Speed * deltaTimeMs;
}