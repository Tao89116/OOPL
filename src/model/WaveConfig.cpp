#include "model/WaveConfig.h"

#include "ResourceManager.h"
#include <stdexcept>
#include <unordered_map>


const WaveConfig& WaveConfig::GetInstance() {
    static WaveConfig instance;
    return instance;
}

WaveConfig::WaveConfig() {
    LoadFromJson();
}

const std::vector<EnemyType>& WaveConfig::GetRoundWave(int round) const {
    if (round < 1 || round > GetTotalRounds()) {
        throw std::runtime_error("Round out of range in wave config: " + std::to_string(round));
    }

    return m_Rounds.at(static_cast<std::size_t>(round - 1));
}

EnemyType WaveConfig::ParseEnemyType(const std::string& typeName) {
    static const std::unordered_map<std::string, EnemyType> typeMap = {
        {"Red", EnemyType::Red},
        {"Blue", EnemyType::Blue},
        {"Green", EnemyType::Green},
        {"Yellow", EnemyType::Yellow},
        {"Black", EnemyType::Black},
        {"White", EnemyType::White},
        {"Lead", EnemyType::Lead},
        {"Rainbow", EnemyType::Rainbow}
    };

    const auto found = typeMap.find(typeName);
    if (found == typeMap.end()) {
        throw std::runtime_error("Invalid enemy type in waves.json: " + typeName);
    }

    return found->second;
}

void WaveConfig::LoadFromJson() {
    const auto& root = ResourceManager::GetInstance().GetJson("waves");

    if (!root.contains("spawnIntervalMs") || !root.at("spawnIntervalMs").is_number_integer()) {
        throw std::runtime_error("Missing or invalid spawnIntervalMs in waves.json.");
    }

    m_SpawnIntervalMs = root.at("spawnIntervalMs").get<int>();
    if (m_SpawnIntervalMs <= 0) {
        throw std::runtime_error("spawnIntervalMs must be positive in waves.json.");
    }

    if (!root.contains("rounds") || !root.at("rounds").is_array()) {
        throw std::runtime_error("Missing or invalid rounds array in waves.json.");
    }

    const auto& roundsJson = root.at("rounds");
    if (roundsJson.size() != ExpectedTotalRounds) {
        throw std::runtime_error(
            "waves.json must contain exactly " + std::to_string(ExpectedTotalRounds) + " rounds."
        );
    }

    m_Rounds.clear();
    m_Rounds.reserve(roundsJson.size());

    for (std::size_t index = 0; index < roundsJson.size(); ++index) {
        const auto& roundJson = roundsJson.at(index);
        const int expectedRound = static_cast<int>(index + 1);

        if (!roundJson.contains("round") || !roundJson.at("round").is_number_integer()) {
            throw std::runtime_error("Missing round number in waves.json at index " + std::to_string(index));
        }

        const int roundNumber = roundJson.at("round").get<int>();
        if (roundNumber != expectedRound) {
            throw std::runtime_error(
                "Round number mismatch in waves.json. Expected " + std::to_string(expectedRound) +
                ", got " + std::to_string(roundNumber) + "."
            );
        }

        if (!roundJson.contains("segments") || !roundJson.at("segments").is_array() ||
            roundJson.at("segments").empty()) {
            throw std::runtime_error("Round " + std::to_string(roundNumber) + " must have at least one segment.");
        }

        std::vector<EnemyType> expandedWave;
        for (const auto& segmentJson : roundJson.at("segments")) {
            if (!segmentJson.contains("type") || !segmentJson.at("type").is_string()) {
                throw std::runtime_error("Missing or invalid segment type in round " + std::to_string(roundNumber));
            }
            if (!segmentJson.contains("count") || !segmentJson.at("count").is_number_integer()) {
                throw std::runtime_error("Missing or invalid segment count in round " + std::to_string(roundNumber));
            }

            const std::string typeName = segmentJson.at("type").get<std::string>();
            const int count = segmentJson.at("count").get<int>();
            if (count <= 0) {
                throw std::runtime_error("Segment count must be positive in round " + std::to_string(roundNumber));
            }

            const EnemyType enemyType = ParseEnemyType(typeName);
            expandedWave.insert(expandedWave.end(), static_cast<std::size_t>(count), enemyType);
        }

        m_Rounds.push_back(std::move(expandedWave));
    }
}
