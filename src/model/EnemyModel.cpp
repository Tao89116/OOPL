//
// Created by polyunicorn on 2026/3/13.
//
#include "model/EnemyModel.h"

#include <algorithm>
#include <cmath>

EnemyModel::EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex)
    : m_Type(type),
      m_Position(spawnPosition),
      m_PathBranchIndex(pathBranchIndex) {
    SetupStatsByType();
}

EnemyModel::EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex, int pathIndex)
    : m_Type(type),
      m_Position(spawnPosition),
      m_PathIndex(std::max(0, pathIndex)),
      m_PathBranchIndex(pathBranchIndex) {
    SetupStatsByType();
}

void EnemyModel::SetupStatsByType() {
    switch (m_Type) {
        case EnemyType::Red:
            m_HP = 1;
            m_BaseSpeed = 0.12f;
            m_Reward = 15;
            m_SpriteKey = "bloon_0";
            break;
        case EnemyType::Blue:
            m_HP = 1;
            m_BaseSpeed = 0.14f;
            m_Reward = 20;
            m_SpriteKey = "bloon_1";
            break;
        case EnemyType::Green:
            m_HP = 1;
            m_BaseSpeed = 0.16f;
            m_Reward = 25;
            m_SpriteKey = "bloon_2";
            break;
        case EnemyType::Yellow:
            m_HP = 1;
            m_BaseSpeed = 0.20f;
            m_Reward = 30;
            m_SpriteKey = "bloon_3";
            break;
    }
}

void EnemyModel::Update(float deltaTimeMs, const std::vector<glm::vec2>& path) {
    if (!m_Alive || m_ReachedGoal || path.size() < 2) {
        return;
    }

    // 每幀先更新狀態時間，並套用速度倍率
    m_SpeedMultiplier = 1.0f;

    if (m_FreezeRemainMs > 0.0f) {
        m_FreezeRemainMs = std::max(0.0f, m_FreezeRemainMs - deltaTimeMs);
        m_SpeedMultiplier = 0.0f;
    }

    if (m_SlowRemainMs > 0.0f) {
        m_SlowRemainMs = std::max(0.0f, m_SlowRemainMs - deltaTimeMs);
        if (m_SpeedMultiplier > 0.0f) {
            m_SpeedMultiplier = std::min(m_SpeedMultiplier, m_SlowMultiplier);
        }
    } else {
        m_SlowMultiplier = 1.0f;
    }

    // 如果目前已經在最後一段之後，直接視為到終點
    if (m_PathIndex >= static_cast<int>(path.size()) - 1) {
        m_ReachedGoal = true;
        m_Alive = false;
        m_ChildrenToSpawn.clear();
        return;
    }

    const glm::vec2 target = path[m_PathIndex + 1];
    const glm::vec2 direction = target - m_Position;
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 保險：如果已經非常接近目標點，直接吸附
    if (distance <= 0.001f) {
        m_Position = target;
        ++m_PathIndex;

        if (m_PathIndex >= static_cast<int>(path.size()) - 1) {
            m_ReachedGoal = true;
            m_Alive = false;
            m_ChildrenToSpawn.clear();
        }
        return;
    }

    // 凍結中：本幀不移動，但狀態時間仍會在上面被遞減
    if (m_SpeedMultiplier <= 0.0f) {
        return;
    }

    const float moveDistance = (m_BaseSpeed * m_SpeedMultiplier) * deltaTimeMs;

    // 這一幀如果會超過 target，就直接吸附到 target，避免來回震盪
    if (moveDistance >= distance) {
        m_Position = target;
        ++m_PathIndex;

        // 如果吸附後已經到最後一個 path 點，立刻消失
        if (m_PathIndex >= static_cast<int>(path.size()) - 1) {
            m_ReachedGoal = true;
            m_Alive = false;
            m_ChildrenToSpawn.clear();
        }
        return;
    }

    const glm::vec2 normalized = direction / distance;
    m_Position += normalized * moveDistance;
}

void EnemyModel::TakeDamage(int damage) {
    if (!m_Alive || damage <= 0) {
        return;
    }

    m_HP -= damage;
    if (m_HP <= 0) {
        m_HP = 0;
        m_Alive = false;
        m_ReachedGoal = false;
        m_ChildrenToSpawn = GetChildrenByType(m_Type);
    }
}

std::optional<EnemyModel::DeathEvent> EnemyModel::ConsumeDeathEvent() {
    if (m_Alive || m_DeathEventConsumed) {
        return std::nullopt;
    }

    m_DeathEventConsumed = true;

    DeathEvent event;
    event.reachedGoal = m_ReachedGoal;
    event.reward = m_ReachedGoal ? 0 : m_Reward;
    event.childrenToSpawn = m_ChildrenToSpawn;
    m_ChildrenToSpawn.clear();
    return event;
}

std::vector<EnemyType> EnemyModel::GetChildrenByType(EnemyType type) {
    switch (type) {
        case EnemyType::Red:
            return {};
        case EnemyType::Blue:
            return {EnemyType::Red};
        case EnemyType::Green:
            return {EnemyType::Blue};
        case EnemyType::Yellow:
            return {EnemyType::Green};
    }
    return {};
}

void EnemyModel::ApplyFreeze(float durationMs) {
    if (!m_Alive || durationMs <= 0.0f) {
        return;
    }

    m_FreezeRemainMs = std::max(m_FreezeRemainMs, durationMs);
}

void EnemyModel::ApplySlow(float speedMultiplier, float durationMs) {
    if (!m_Alive || durationMs <= 0.0f) {
        m_isSlowed = false;
        return;
    }
    if (durationMs > 0.0f && m_isSlowed == true) {
        return;
    }

    const float clampedMultiplier = std::clamp(speedMultiplier, 0.1f, 1.0f);
    m_SlowMultiplier = std::min(m_SlowMultiplier, clampedMultiplier);
    m_SlowRemainMs = std::max(m_SlowRemainMs, durationMs);
    m_isSlowed = true;
}
