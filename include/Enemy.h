//
// Created by polyunicorn on 2026/3/9.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ENEMY_H
#define REPLACE_WITH_YOUR_PROJECT_NAME_ENEMY_H

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Enemy : public Util::GameObject {
public:
    Enemy(const std::string& imagePath, const glm::vec2& startPos);

    void Update(float deltaTimeMs);
    void TakeDamage(int damage);

    bool IsDead() const { return m_HP <= 0; }
    glm::vec2 GetPosition() const { return m_Transform.translation; }
    int GetHP() const { return m_HP; }

private:
    int m_HP = 30;
    float m_Speed = 0.0F;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ENEMY_H