//
// Created by polyunicorn on 2026/3/13.
//

#ifndef MAP_MODEL_H
#define MAP_MODEL_H

#include "pch.hpp"
#include "SceneType.h"
#include "model/MapData.h"

class MapModel {
public:
    explicit MapModel(DifficultyType difficulty);
    void ReloadByDifficulty(DifficultyType difficulty);

    const std::vector<std::vector<glm::vec2>>& GetPaths() const { return m_Data.paths; }
    const std::vector<glm::vec2>& GetPath(size_t pathIndex) const { return m_Data.paths.at(pathIndex); }

    size_t GetPathCount() const { return m_Data.paths.size(); }

    glm::vec2 GetSpawnPoint(size_t pathIndex) const { return m_Data.paths.at(pathIndex).front(); }
    float GetPathWidth() const { return m_Data.pathWidth; }
    const std::string& GetBackgroundKey() const { return m_Data.backgroundKey; }

    bool IsCircleOverlappingAnyPath(const glm::vec2& center, float radius) const;
    bool IsCircleOverlappingPath(const glm::vec2& center, float radius, size_t pathIndex) const;

private:
    static float DistancePointToSegment(
        const glm::vec2& point,
        const glm::vec2& segStart,
        const glm::vec2& segEnd
    );

private:
    MapData m_Data;
};

#endif
