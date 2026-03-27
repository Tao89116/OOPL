//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "pch.hpp"
#include "SceneType.h"
#include "model/BuildableRegistry.h"
#include "model/EnemyModel.h"
#include "model/MapModel.h"
#include "model/PlacementModel.h"
#include "model/ProjectileModel.h"
#include "model/TowerBase.h"

class GameModel {
public:
    explicit GameModel(DifficultyType difficulty);

    void Reset();
    void Update(float deltaTimeMs);

    void SelectBuildable(const std::shared_ptr<IBuildableDefinition>& definition);
    void SetMessage(const std::string& message) { m_Message = message; }

    void BeginPlacement(const std::shared_ptr<IBuildableDefinition>& definition);
    void CancelPlacement();
    void UpdatePlacementPreview(const glm::vec2& worldPos);
    bool ConfirmPlacement();

    bool CanPlaceBuildable(const std::shared_ptr<IBuildableDefinition>& definition, const glm::vec2& position) const;

    void StartRound();
    void TogglePause();

    //cheat
    void ForceWin();
    void ForceLose();

    int GetHP() const { return m_HP; }
    int GetGold() const { return m_Gold; }
    int GetRound() const { return m_Round; }
    int GetTotalRounds() const { return m_TotalRounds; }

    bool IsRoundRunning() const { return m_RoundRunning; }
    bool IsPaused() const { return m_Paused; }
    bool IsWin() const { return m_Win; }
    bool IsLose() const { return m_Lose; }

    DifficultyType GetDifficulty() const { return m_Difficulty; }
    const std::shared_ptr<IBuildableDefinition>& GetSelectedBuildableDefinition() const { return m_SelectedBuildableDefinition; }
    const std::string& GetMessage() const { return m_Message; }

    const MapModel& GetMap() const { return m_Map; }
    const PlacementModel& GetPlacement() const { return m_Placement; }

    const std::vector<std::shared_ptr<TowerBase>>& GetTowers() const { return m_Towers; }
    const std::vector<std::shared_ptr<EnemyModel>>& GetEnemies() const { return m_Enemies; }
    const std::vector<std::shared_ptr<ProjectileModel>>& GetProjectiles() const { return m_Projectiles; }

private:
    struct PlacementCheckResult {
        bool valid = false;
        std::string hintText;
    };

    PlacementCheckResult EvaluatePlacement(
        const std::shared_ptr<IBuildableDefinition>& definition,
        const glm::vec2& position
    ) const;

    void SetupDifficulty();
    void SetupWave();
    void SpawnEnemy();
    void UpdateTowers(float deltaTimeMs);
    void UpdateEnemies(float deltaTimeMs);
    void UpdateProjectiles(float deltaTimeMs);
    void CleanupObjects();
    void CheckEndState();

private:
    DifficultyType m_Difficulty;
    MapModel m_Map;
    PlacementModel m_Placement;

    int m_HP = 20;
    int m_Gold = 300;
    int m_Round = 1;
    int m_TotalRounds = 5;

    bool m_RoundRunning = false;
    bool m_Paused = false;
    bool m_Win = false;
    bool m_Lose = false;

    std::shared_ptr<IBuildableDefinition> m_SelectedBuildableDefinition = nullptr;
    std::string m_Message = "Press 1/2/3 to choose tower, click to place.";

    std::vector<std::shared_ptr<TowerBase>> m_Towers;
    std::vector<std::shared_ptr<EnemyModel>> m_Enemies;
    std::vector<std::shared_ptr<ProjectileModel>> m_Projectiles;

    std::vector<EnemyType> m_CurrentWave;
    int m_SpawnedCount = 0;
    float m_SpawnTimerMs = 0.0f;
    float m_SpawnIntervalMs = 900.0f;
};

#endif