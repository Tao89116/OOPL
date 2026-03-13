//
// Created by polyunicorn on 2026/3/9.
//
#include "Enemy.h"

Enemy::Enemy(const std::string& imagePath, const glm::vec2& startPos)
    : Util::GameObject(std::make_shared<Util::Image>(imagePath), 10.0F) {
    m_Transform.translation = startPos;
    m_Transform.scale = {0.2F, 0.2F};
}

void Enemy::Update(float deltaTimeMs) {
    (void)deltaTimeMs;
}

void Enemy::TakeDamage(int damage) {
    m_HP -= damage;
    if (m_HP < 0) {
        m_HP = 0;
    }
}