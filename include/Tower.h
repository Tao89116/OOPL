//
// Created by polyunicorn on 2026/3/9.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TOWER_H
#define REPLACE_WITH_YOUR_PROJECT_NAME_TOWER_H

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Tower : public Util::GameObject {
public:
    Tower(const std::string& imagePath, const glm::vec2& pos);

    void Update(float deltaTimeMs);

    glm::vec2 GetPosition() const { return m_Transform.translation; }
    float GetRange() const { return m_Range; }
    int GetDamage() const { return m_Damage; }

private:
    float m_Range = 160.0F;
    float m_AttackIntervalMs = 700.0F;
    int m_Damage = 10;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TOWER_H