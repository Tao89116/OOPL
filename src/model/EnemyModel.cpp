//
// Created by polyunicorn on 2026/3/13.
//
#include "model/EnemyModel.h"
#include "model/StatusEffects/BurnEffect.h"
#include "model/StatusEffects/FreezeEffect.h"
#include "model/StatusEffects/PoisonEffect.h"
#include "model/StatusEffects/SlowEffect.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <unordered_map>

namespace {
struct EnemyProfile {
    int hp;
    float speed;
    int reward;
    int leakDamage;
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
    static EnemyProfile Build() { return {1, 0.08f, 1, 1, "bloon_0", {}}; }
};

template <>
struct EnemyProfileTraits<BlueTag> {
    static constexpr EnemyType Type = EnemyType::Blue;
    static EnemyProfile Build() { return {1, 0.08*1.4f, 2, 0, "bloon_1", {EnemyType::Red}}; }
};

template <>
struct EnemyProfileTraits<GreenTag> {
    static constexpr EnemyType Type = EnemyType::Green;
    static EnemyProfile Build() { return {1, 0.08*1.8f, 3, 0, "bloon_2", {EnemyType::Blue}}; }
};

template <>
struct EnemyProfileTraits<YellowTag> {
    static constexpr EnemyType Type = EnemyType::Yellow;
    static EnemyProfile Build() { return {1, 0.08*3.2f, 4, 0, "bloon_3", {EnemyType::Green}}; }
};

template <>
struct EnemyProfileTraits<BlackTag> {
    static constexpr EnemyType Type = EnemyType::Black;
    static EnemyProfile Build() { return {1, 0.08*1.8f, 5, 0, "bloon_4", {EnemyType::Yellow, EnemyType::Yellow}}; }
};

template <>
struct EnemyProfileTraits<WhiteTag> {
    static constexpr EnemyType Type = EnemyType::White;
    static EnemyProfile Build() { return {1, 0.08*2.5f, 5, 0, "bloon_5", {EnemyType::Yellow, EnemyType::Yellow}}; }
};

template <>
struct EnemyProfileTraits<LeadTag> {
    static constexpr EnemyType Type = EnemyType::Lead;
    static EnemyProfile Build() { return {1, 0.08f, 10, 0, "bloon_6", {EnemyType::Black, EnemyType::Black}}; }
};

template <>
struct EnemyProfileTraits<RainbowTag> {
    static constexpr EnemyType Type = EnemyType::Rainbow;
    static EnemyProfile Build() {
        return {1, 0.08*2.2f, 30, 0, "bloon_7", {EnemyType::Black, EnemyType::Black, EnemyType::White, EnemyType::White}};
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

namespace {
class BasicDamageRule final : public EnemyModel::DamageRule {
public:
    bool CanDamage(const EnemyModel& enemy) const override {
        return enemy.CanBeTargeted() &&
               !enemy.IsFrozen() &&
               enemy.GetType() != EnemyType::Lead;
    }
};

class FrozenCapableDamageRule final : public EnemyModel::DamageRule {
public:
    bool CanDamage(const EnemyModel& enemy) const override {
        return enemy.CanBeTargeted() && enemy.GetType() != EnemyType::Lead;
    }
};

class ExplosiveDamageRule final : public EnemyModel::DamageRule {
public:
    bool CanDamage(const EnemyModel& enemy) const override {
        return enemy.CanBeTargeted() && enemy.GetType() != EnemyType::Black;
    }
};
}

const EnemyModel::DamageRule& EnemyModel::BasicDamageRule() {
    static const ::BasicDamageRule rule;
    return rule;
}

const EnemyModel::DamageRule& EnemyModel::FrozenCapableDamageRule() {
    static const ::FrozenCapableDamageRule rule;
    return rule;
}

const EnemyModel::DamageRule& EnemyModel::ExplosiveDamageRule() {
    static const ::ExplosiveDamageRule rule;
    return rule;
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
    m_LeakDamage = GetRBE(type);
    m_SpriteKey = profile.sprite;
}

void EnemyModel::Update(float deltaTimeMs, const std::vector<glm::vec2>& path) {
    if (!m_Alive || m_ReachedGoal) {
        return;
    }

    // 每幀更新所有狀態效果、清除過期效果，並彙整速度倍率。
    m_SpeedMultiplier = 1.0f;
    for (const auto& statusEffect : m_StatusEffects) {
        statusEffect->Update(*this, deltaTimeMs);
    }

    m_StatusEffects.erase(
        std::remove_if(
            m_StatusEffects.begin(),
            m_StatusEffects.end(),
            [](const std::unique_ptr<IStatusEffect>& statusEffect) {
                return statusEffect->IsExpired();
            }),
        m_StatusEffects.end());

    for (const auto& statusEffect : m_StatusEffects) {
        m_SpeedMultiplier = std::min(m_SpeedMultiplier, statusEffect->GetSpeedMultiplier());
    }

    if (!m_Alive || m_ReachedGoal || path.size() < 2) {
        return;
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

void EnemyModel::TakeDamage(int damage, const DamageRule& damageRule) {
    if (damage <= 0 || !CanReceiveDamage(damageRule)) {
        return;
    }

    TakePureDamage(damage);
}

void EnemyModel::TakePureDamage(int damage) {
    if (damage <= 0 || !CanBeTargeted()) {
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

bool EnemyModel::CanReceiveDamage(const DamageRule& damageRule) const {
    return damageRule.CanDamage(*this);
}

bool EnemyModel::CanReceiveFreeze() const {
    return CanBeTargeted() && m_Type != EnemyType::White;
}

std::optional<EnemyModel::DeathEvent> EnemyModel::ConsumeDeathEvent() {
    if (m_Alive || m_DeathEventConsumed) {
        return std::nullopt;
    }

    m_DeathEventConsumed = true;

    DeathEvent event;
    event.reachedGoal = m_ReachedGoal;
    event.reward = m_ReachedGoal ? 0 : m_Reward;
    event.childrenToSpawn = m_ReachedGoal ? std::vector<EnemyType>{} : m_ChildrenToSpawn;
    m_ChildrenToSpawn.clear();
    return event;
}

std::vector<EnemyType> EnemyModel::GetChildrenByType(EnemyType type) {
    return GetProfile(type).childrenOnPop;
}

int EnemyModel::GetRBE(EnemyType type) {
    const EnemyProfile& profile = GetProfile(type);
    if (profile.leakDamage > 0) {
        return profile.leakDamage;
    }

    int rbe = 1;
    for (EnemyType childType : profile.childrenOnPop) {
        rbe += GetRBE(childType);
    }
    return rbe;
}

void EnemyModel::ApplyFreeze(float durationMs) {
    if (durationMs <= 0.0f || !CanReceiveFreeze()) {
        return;
    }

    AddStatusEffect(std::make_unique<FreezeEffect>(durationMs));
}

void EnemyModel::ApplySlow(float speedMultiplier, float durationMs) {
    if (!CanBeTargeted() || durationMs <= 0.0f) {
        return;
    }

    AddStatusEffect(std::make_unique<SlowEffect>(speedMultiplier, durationMs));
}

void EnemyModel::AddStatusEffect(std::unique_ptr<IStatusEffect> statusEffect) {
    if (!statusEffect || !CanBeTargeted()) {
        return;
    }

    statusEffect->OnApply(*this);
    if (!statusEffect->IsExpired()) {
        m_StatusEffects.push_back(std::move(statusEffect));
    }
}

bool EnemyModel::IsFrozen() const {
    return std::any_of(
        m_StatusEffects.begin(),
        m_StatusEffects.end(),
        [](const std::unique_ptr<IStatusEffect>& statusEffect) {
            return !statusEffect->IsExpired() && statusEffect->BlocksDamage();
        });
}

void FreezeEffect::OnApply(EnemyModel&) {
}

void FreezeEffect::Update(EnemyModel&, float deltaTimeMs) {
    m_RemainingMs = std::max(0.0f, m_RemainingMs - deltaTimeMs);
}

void SlowEffect::OnApply(EnemyModel&) {
}

void SlowEffect::Update(EnemyModel&, float deltaTimeMs) {
    m_RemainingMs = std::max(0.0f, m_RemainingMs - deltaTimeMs);
}

void BurnEffect::OnApply(EnemyModel&) {
}

void BurnEffect::Update(EnemyModel& enemy, float deltaTimeMs) {
    if (IsExpired()) {
        return;
    }

    const float appliedDeltaTimeMs = std::min(deltaTimeMs, m_RemainingMs);
    m_RemainingMs = std::max(0.0f, m_RemainingMs - deltaTimeMs);
    m_ElapsedSinceTickMs += appliedDeltaTimeMs;

    while (m_ElapsedSinceTickMs >= m_TickIntervalMs && m_DamagePerTick > 0) {
        enemy.TakePureDamage(m_DamagePerTick);
        m_ElapsedSinceTickMs -= m_TickIntervalMs;
    }
}

void PoisonEffect::OnApply(EnemyModel&) {
}

void PoisonEffect::Update(EnemyModel& enemy, float deltaTimeMs) {
    if (IsExpired()) {
        return;
    }

    const float appliedDeltaTimeMs = std::min(deltaTimeMs, m_RemainingMs);
    m_RemainingMs = std::max(0.0f, m_RemainingMs - deltaTimeMs);
    m_ElapsedSinceTickMs += appliedDeltaTimeMs;

    while (m_ElapsedSinceTickMs >= m_TickIntervalMs && m_DamagePerTick > 0) {
        enemy.TakePureDamage(m_DamagePerTick);
        m_ElapsedSinceTickMs -= m_TickIntervalMs;
    }
}
