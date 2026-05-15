#ifndef WAVE_CONFIG_H
#define WAVE_CONFIG_H

#include "model/EnemyModel.h"
#include "pch.hpp"

class WaveConfig {
public:
    static constexpr int ExpectedTotalRounds = 50;

    static const WaveConfig& GetInstance();

    int GetSpawnIntervalMs() const { return m_SpawnIntervalMs; }
    int GetTotalRounds() const { return static_cast<int>(m_Rounds.size()); }
    const std::vector<EnemyType>& GetRoundWave(int round) const;

private:
    WaveConfig();

    void LoadFromJson();
    static EnemyType ParseEnemyType(const std::string& typeName);

private:
    int m_SpawnIntervalMs = 400;
    std::vector<std::vector<EnemyType>> m_Rounds;
};

#endif
