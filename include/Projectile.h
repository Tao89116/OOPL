//
// Created by polyunicorn on 2026/3/12.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_PROJECTILE_H
#define REPLACE_WITH_YOUR_PROJECT_NAME_PROJECTILE_H

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Projectile : public Util::GameObject {
public:
    Projectile(const std::string& imagePath, const glm::vec2& startPos);

    void Update(float deltaTimeMs);

    bool IsActive() const { return m_IsActive; }
    void SetActive(bool active) { m_IsActive = active; }

private:
    bool m_IsActive = false;
    float m_Speed = 0.35F;
};


#endif //REPLACE_WITH_YOUR_PROJECT_NAME_PROJECTILE_H