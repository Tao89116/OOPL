//
// Created by polyunicorn on 2026/3/13.
//
#include "model/EnemyModel.h"

#include <cmath>

EnemyModel::EnemyModel(EnemyType type, const glm::vec2& startPos)
    : m_Type(type), m_Position(startPos) {
    SetupStatsByType();
}

void EnemyModel::SetupStatsByType() {
    switch (m_Type) {
        case EnemyType::Red:
            m_HP = 1;
            m_Speed = 0.12f;
            m_Reward = 15;
            m_SpriteKey = "bloon_0";
            break;
        case EnemyType::Blue:
            m_HP = 2;
            m_Speed = 0.14f;
            m_Reward = 20;
            m_SpriteKey = "bloon_1";
            break;
        case EnemyType::Green:
            m_HP = 3;
            m_Speed = 0.16f;
            m_Reward = 25;
            m_SpriteKey = "bloon_2";
            break;
        case EnemyType::Yellow:
            m_HP = 4;
            m_Speed = 0.20f;
            m_Reward = 30;
            m_SpriteKey = "bloon_3";
            break;
    }
}

void EnemyModel::Update(float deltaTimeMs, const std::vector<glm::vec2>& path) {
    if (!m_Alive || m_ReachedGoal || path.size() < 2) {
        return;
    }

    if (m_PathIndex >= static_cast<int>(path.size()) - 1) {
        m_ReachedGoal = true;
        m_Alive = false;
        return;
    }

    const glm::vec2 target = path[m_PathIndex + 1];
    const glm::vec2 direction = target - m_Position;
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f) {
        m_Position = target;
        ++m_PathIndex;

        if (m_PathIndex >= static_cast<int>(path.size()) - 1) {
            m_ReachedGoal = true;
            m_Alive = false;
        }
        return;
    }

    const glm::vec2 normalized = direction / distance;
    m_Position += normalized * m_Speed * deltaTimeMs;
}

void EnemyModel::TakeDamage(int damage) {
    if (!m_Alive) {
        return;
    }

    m_HP -= damage;
    if (m_HP <= 0) {
        m_HP = 0;
        m_Alive = false;
    }
}