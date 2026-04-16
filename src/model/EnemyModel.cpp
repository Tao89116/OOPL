//
// Created by polyunicorn on 2026/3/13.
//
#include "model/EnemyModel.h"

#include <algorithm>
#include <cmath>
#include <unordered_map>

namespace {
struct EnemyProfile {
    int hp;
    float speed;
    int reward;
    const char* sprite;
    std::vector<EnemyType> childrenOnPop;//氣球破掉邏輯 高級氣球會分裂成更多氣球
};

template <typename TTag>
struct EnemyProfileTraits;

struct RedTag {};
struct BlueTag {};
struct GreenTag {};
struct YellowTag {};
struct BlackTag {};
struct WhiteTag {};
struct LeadTag {};
struct RainbowTag {};

template <>
struct EnemyProfileTraits<RedTag> {
    static constexpr EnemyType Type = EnemyType::Red;
    static EnemyProfile Build() { return {1, 0.12f, 15, "bloon_0", {}}; }
};

template <>
struct EnemyProfileTraits<BlueTag> {
    static constexpr EnemyType Type = EnemyType::Blue;
    static EnemyProfile Build() { return {1, 0.14f, 20, "bloon_1", {EnemyType::Red}}; }
};

template <>
struct EnemyProfileTraits<GreenTag> {
    static constexpr EnemyType Type = EnemyType::Green;
    static EnemyProfile Build() { return {1, 0.16f, 25, "bloon_2", {EnemyType::Blue}}; }
};

template <>
struct EnemyProfileTraits<YellowTag> {
    static constexpr EnemyType Type = EnemyType::Yellow;
    static EnemyProfile Build() { return {1, 0.20f, 30, "bloon_3", {EnemyType::Green}}; }
};

template <>
struct EnemyProfileTraits<BlackTag> {
    static constexpr EnemyType Type = EnemyType::Black;
    static EnemyProfile Build() { return {1, 0.21f, 35, "bloon_4", {EnemyType::Yellow, EnemyType::Yellow}}; }
};

template <>
struct EnemyProfileTraits<WhiteTag> {
    static constexpr EnemyType Type = EnemyType::White;
    static EnemyProfile Build() { return {1, 0.21f, 35, "bloon_5", {EnemyType::Yellow, EnemyType::Yellow}}; }
};

template <>
struct EnemyProfileTraits<LeadTag> {
    static constexpr EnemyType Type = EnemyType::Lead;
    static EnemyProfile Build() { return {2, 0.10f, 45, "bloon_6", {EnemyType::Black, EnemyType::Black}}; }
};

template <>
struct EnemyProfileTraits<RainbowTag> {
    static constexpr EnemyType Type = EnemyType::Rainbow;
    static EnemyProfile Build() {
        return {1, 0.22f, 60, "bloon_7", {EnemyType::Black, EnemyType::Black, EnemyType::White, EnemyType::White}};
    }
};

template <typename TTag>
void RegisterEnemyProfile(std::unordered_map<EnemyType, EnemyProfile>& profiles) {
    profiles.emplace(EnemyProfileTraits<TTag>::Type, EnemyProfileTraits<TTag>::Build());
}

template <typename... TTags>
std::unordered_map<EnemyType, EnemyProfile> BuildEnemyProfiles() {
    std::unordered_map<EnemyType, EnemyProfile> profiles;
    (RegisterEnemyProfile<TTags>(profiles), ...);
    return profiles;
}

const EnemyProfile& GetProfile(EnemyType type) {
    static const std::unordered_map<EnemyType, EnemyProfile> kProfiles =
        BuildEnemyProfiles<RedTag, BlueTag, GreenTag, YellowTag, BlackTag, WhiteTag, LeadTag, RainbowTag>();

    const auto it = kProfiles.find(type);
    if (it == kProfiles.end()) {
        return kProfiles.at(EnemyType::Red);
    }
    return it->second;
}
}

EnemyModel::EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex)
    : m_Type(type),
      m_Position(spawnPosition),
      m_PathBranchIndex(pathBranchIndex) {
    SetupStatsByType(type);
}

EnemyModel::EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex, int pathIndex)
    : m_Type(type),
      m_Position(spawnPosition),
      m_PathIndex(std::max(0, pathIndex)),
      m_PathBranchIndex(pathBranchIndex) {
    SetupStatsByType(type);
}

void EnemyModel::SetupStatsByType(EnemyType type) {
    const EnemyProfile& profile = GetProfile(type);
    m_HP = profile.hp;
    m_BaseSpeed = profile.speed;
    m_Reward = profile.reward;
    m_SpriteKey = profile.sprite;
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
    return GetProfile(type).childrenOnPop;
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
