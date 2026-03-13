//
// Created by polyunicorn on 2026/3/12.
//
#include "Projectile.h"

Projectile::Projectile(const std::string& imagePath, const glm::vec2& startPos)
    : Util::GameObject(std::make_shared<Util::Image>(imagePath), 15.0F) {
    m_Transform.translation = startPos;
    m_Transform.scale = {0.08F, 0.08F};
}

void Projectile::Update(float deltaTimeMs) {
    m_Transform.translation.x += m_Speed * deltaTimeMs;
    if (!m_IsActive) {
        return;
    }
}