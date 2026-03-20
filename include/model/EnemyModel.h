//
// Created by polyunicorn on 2026/3/13.
//

#ifndef ENEMY_MODEL_H
#define ENEMY_MODEL_H

#include "pch.hpp"

enum class EnemyType {
    Red,
    Blue,
    Green,
    Yellow
};

class EnemyModel {
public:
    EnemyModel(EnemyType type, const glm::vec2& spawnPosition, int pathBranchIndex);

    void Update(float deltaTimeMs, const std::vector<glm::vec2>& path);
    void TakeDamage(int damage);

    bool IsAlive() const { return m_Alive; }
    bool HasReachedGoal() const { return m_ReachedGoal; }
    bool CanBeTargeted() const { return m_Alive && !m_ReachedGoal; }

    const glm::vec2& GetPosition() const { return m_Position; }
    EnemyType GetType() const { return m_Type; }
    int GetHP() const { return m_HP; }
    int GetReward() const { return m_Reward; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }

    int GetPathBranchIndex() const { return m_PathBranchIndex; }
private:
    void SetupStatsByType();

private:
    EnemyType m_Type;
    glm::vec2 m_Position;
    int m_PathIndex = 0;
    int m_PathBranchIndex = 0;    // 這隻敵人屬於哪一條 path
    int m_HP = 1;
    float m_Speed = 0.12f;
    int m_Reward = 15;

    bool m_Alive = true;
    bool m_ReachedGoal = false;

    std::string m_SpriteKey;

};

#endif