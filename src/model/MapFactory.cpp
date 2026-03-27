#include "model/MapFactory.h"

#include "ResourceManager.h"
#include <stdexcept>

namespace {
    glm::vec2 ParsePoint(const nlohmann::json& pointJson) {
        if (!pointJson.is_array() || pointJson.size() != 2) {
            throw std::runtime_error("Invalid point format in map json.");
        }

        return {
            pointJson[0].get<float>(),
            pointJson[1].get<float>()
        };
    }
}

const char* MapFactory::DifficultyToJsonKey(DifficultyType difficulty) {
    switch (difficulty) {
        case DifficultyType::Easy:
            return "easy";
        case DifficultyType::Normal:
            return "normal";
        case DifficultyType::Hard:
            return "hard";
    }
    return "easy";
}

MapData MapFactory::CreateByDifficulty(DifficultyType difficulty) {
    ResourceManager& resources = ResourceManager::GetInstance();
    const auto& root = resources.GetJson("map_paths");

    const std::string difficultyKey = DifficultyToJsonKey(difficulty);

    if (!root.contains(difficultyKey)) {
        throw std::runtime_error("Difficulty key not found in map_paths.json: " + difficultyKey);
    }

    const auto& mapJson = root.at(difficultyKey);

    MapData data;

    if (!mapJson.contains("background")) {
        throw std::runtime_error("Missing background in map json.");
    }
    data.backgroundKey = mapJson.at("background").get<std::string>();

    if (!mapJson.contains("paths") || !mapJson.at("paths").is_array()) {
        throw std::runtime_error("Missing or invalid paths in map json.");
    }

    for (const auto& pathJson : mapJson.at("paths")) {
        std::vector<glm::vec2> pathPoints;

        if (!pathJson.is_array()) {
            throw std::runtime_error("Invalid path format in map json.");
        }

        for (const auto& pointJson : pathJson) {
            pathPoints.push_back(ParsePoint(pointJson));
        }

        data.paths.push_back(pathPoints);
    }

    return data;
}