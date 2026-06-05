//
// Created by polyunicorn on 2026/3/13.
//

#ifndef ENEMY_MODEL_H
#define ENEMY_MODEL_H

#include "pch.hpp"
#include <optional>

enum class EnemyType {
    Red,
    Blue,
    Green,
    Yellow,
    Black,
    White,
    Lead,
    Rainbow
};

class EnemyModel {
public:
    struct DeathEvent {
        bool reachedGoal = false;
        int reward = 0;
        std::vector<EnemyType> childrenToSpawn;
    };

    struct DamageOptions {
        bool canPopFrozen = false;
        bool canPopLead = false;
        bool isCannonDamage = false;
    };

    EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex);
    EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex, int pathIndex);

    void Update(float deltaTimeMs, const std::vector<glm::vec2>& path);
    void TakeDamage(int damage, const DamageOptions& options = {});
    bool CanReceiveDamage(const DamageOptions& options = {}) const;
    void ApplyFreeze(float durationMs);
    void ApplySlow(float speedMultiplier, float durationMs);

    bool IsAlive() const { return m_Alive; }
    bool IsFrozen() const { return m_FreezeRemainMs > 0.0f; }
    bool HasReachedGoal() const { return m_ReachedGoal; }
    bool CanBeTargeted() const { return m_Alive && !m_ReachedGoal; }

    const glm::vec2& GetPosition() const { return m_Position; }
    EnemyType GetType() const { return m_Type; }
    int GetHP() const { return m_HP; }
    int GetReward() const { return m_Reward; }
    int GetLeakDamage() const { return m_LeakDamage; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }

    int GetPathBranchIndex() const { return m_PathBranchIndex; }
    int GetPathIndex() const { return m_PathIndex; }
    static int GetRBE(EnemyType type);
    std::optional<DeathEvent> ConsumeDeathEvent();
private:
    void SetupStatsByType(EnemyType type);
    static std::vector<EnemyType> GetChildrenByType(EnemyType type);

private:
    EnemyType m_Type;
    glm::vec2 m_Position;
    int m_PathIndex = 0;
    int m_PathBranchIndex = 0;    // 這隻敵人屬於哪一條 path
    int m_HP = 1;
    float m_BaseSpeed = 0.12f;
    float m_SpeedMultiplier = 1.0f;
    float m_SlowMultiplier = 1.0f;
    float m_FreezeRemainMs = 0.0f;
    float m_SlowRemainMs = 0.0f;
    int m_Reward = 15;
    int m_LeakDamage = 1;

    bool m_Alive = true;
    bool m_ReachedGoal = false;
    bool m_isSlowed = false;
    bool m_DeathEventConsumed = false;
    std::vector<EnemyType> m_ChildrenToSpawn;

    std::string m_SpriteKey;

};

#endif
