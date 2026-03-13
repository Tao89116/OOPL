//
// Created by polyunicorn on 2026/3/9.
//
#include "Tower.h"
#include "Enemy.h"

#include <cmath>

Tower::Tower(const std::string& imagePath, const glm::vec2& pos)
    : Util::GameObject(std::make_shared<Util::Image>(imagePath), 20.0F) {
    m_Transform.translation = pos;
    m_Transform.scale = {0.2F, 0.2F};
}
//後面要把建構子的pos移到建塔
void Tower::Update(float deltaTimeMs) {
    (void)deltaTimeMs;
}