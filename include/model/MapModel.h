//
// Created by polyunicorn on 2026/3/13.
//

#ifndef MAP_MODEL_H
#define MAP_MODEL_H

#include "pch.hpp"

class MapModel {
public:
    MapModel();

    const std::vector<glm::vec2>& GetPathPoints() const { return m_PathPoints; }
    const std::vector<glm::vec2>& GetTowerSlots() const { return m_TowerSlots; }

    glm::vec2 GetSpawnPoint() const { return m_PathPoints.front(); }

private:
    std::vector<glm::vec2> m_PathPoints;
    std::vector<glm::vec2> m_TowerSlots;
};

#endif