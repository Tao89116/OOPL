//
// Created by polyunicorn on 2026/3/13.
//

#include "model/MapModel.h"
#include "model/MapFactory.h"

MapModel::MapModel(DifficultyType difficulty)
    : m_Data(MapFactory::CreateByDifficulty(difficulty)) {
}

float MapModel::DistancePointToSegment(
    const glm::vec2& point,
    const glm::vec2& segStart,
    const glm::vec2& segEnd
) {
    const glm::vec2 seg = segEnd - segStart;
    const glm::vec2 toPoint = point - segStart;

    const float segLenSq = seg.x * seg.x + seg.y * seg.y;
    if (segLenSq <= 0.0001f) {
        const glm::vec2 delta = point - segStart;
        return std::sqrt(delta.x * delta.x + delta.y * delta.y);
    }

    float t = (toPoint.x * seg.x + toPoint.y * seg.y) / segLenSq;
    t = std::clamp(t, 0.0f, 1.0f);

    const glm::vec2 closest = segStart + seg * t;
    const glm::vec2 delta = point - closest;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}

bool MapModel::IsCircleOverlappingPath(const glm::vec2& center, float radius, size_t pathIndex) const {
    const auto& path = m_Data.paths.at(pathIndex);
    if (path.size() < 2) {
        return false;
    }

    const float collisionDistance = (m_Data.pathWidth * 0.5f) + radius;

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        const float distance = DistancePointToSegment(center, path[i], path[i + 1]);
        if (distance <= collisionDistance) {
            return true;
        }
    }

    return false;
}

bool MapModel::IsCircleOverlappingAnyPath(const glm::vec2& center, float radius) const {
    for (size_t i = 0; i < m_Data.paths.size(); ++i) {
        if (IsCircleOverlappingPath(center, radius, i)) {
            return true;
        }
    }
    return false;
}