//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "pch.hpp"
#include "SceneType.h"
#include "model/EnemyModel.h"
#include "model/MapModel.h"
#include "model/ProjectileModel.h"
#include "model/TowerModel.h"

class GameModel {
public:
    explicit GameModel(DifficultyType difficulty);

    void Reset();
    void Update(float deltaTimeMs);

    void SelectTower(TowerType type);
    bool PlaceTowerAtSlot(int slotIndex);
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
    TowerType GetSelectedTowerType() const { return m_SelectedTowerType; }
    const std::string& GetMessage() const { return m_Message; }

    const MapModel& GetMap() const { return m_Map; }
    const std::vector<std::shared_ptr<TowerModel>>& GetTowers() const { return m_Towers; }
    const std::vector<std::shared_ptr<EnemyModel>>& GetEnemies() const { return m_Enemies; }
    const std::vector<std::shared_ptr<ProjectileModel>>& GetProjectiles() const { return m_Projectiles; }
    const std::vector<bool>& GetSlotOccupied() const { return m_SlotOccupied; }

private:
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

    int m_HP = 20;
    int m_Gold = 300;
    int m_Round = 1;
    int m_TotalRounds = 5;

    bool m_RoundRunning = false;
    bool m_Paused = false;
    bool m_Win = false;
    bool m_Lose = false;

    TowerType m_SelectedTowerType = TowerType::Dart;
    std::string m_Message = "Press SPACE to start round.";

    std::vector<bool> m_SlotOccupied;
    std::vector<std::shared_ptr<TowerModel>> m_Towers;
    std::vector<std::shared_ptr<EnemyModel>> m_Enemies;
    std::vector<std::shared_ptr<ProjectileModel>> m_Projectiles;

    std::vector<EnemyType> m_CurrentWave;
    int m_SpawnedCount = 0;
    float m_SpawnTimerMs = 0.0f;
    float m_SpawnIntervalMs = 900.0f;
};

#endif