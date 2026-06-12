#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "pch.hpp"
#include "SceneType.h"
#include "model/BuildableRegistry.h"
#include "model/EnemyModel.h"
#include "model/HitEffectEvent.h"
#include "model/IGameState.h"
#include "model/IBuildable.h"
#include "model/MapModel.h"
#include "model/PlacementModel.h"
#include "model/ProjectileModel.h"
#include <memory>

class GameModel : private IGameStateContext {
public:
    struct PoppedEnemyEvent {
        const EnemyModel* enemy = nullptr;
        glm::vec2 position = {0.0f, 0.0f};
    };

    using HitEffectEvent = ::HitEffectEvent;

    explicit GameModel(DifficultyType difficulty);

    void Reset();
    void Update(float deltaTimeMs);

    void SelectBuildable(const BuildableRegistry::Entry* entry);
    void SetHoveredBuildable(const BuildableRegistry::Entry* entry) { m_HoveredBuildableEntry = entry; }
    void SetMessage(const std::string& message) { m_Message = message; }

    void BeginPlacement(const BuildableRegistry::Entry* entry);
    void CancelPlacement();
    void UpdatePlacementPreview(const glm::vec2& worldPos);
    bool ConfirmPlacement();
    bool SelectPlacedTowerAt(const glm::vec2& worldPos);
    void ClearSelectedPlacedTower();
    bool SellSelectedTower();
    bool UpgradeSelectedTower(int upgradePathIndex);

    bool CanPlaceBuildable(const std::shared_ptr<IBuildable>& buildable, const glm::vec2& position) const;

    void StartRound();
    void TogglePause();

    void ForceWin();
    void ForceLose();
    bool SetDifficultyCheat(DifficultyType difficulty);
    bool SetRoundCheat(int round);
    bool SetGoldCheat(int gold);
    bool SetHPCheat(int hp);
    bool SpawnEnemyCheat(EnemyType type, int count);

    int ConsumePoppedBloonCount();
    std::vector<PoppedEnemyEvent> ConsumePoppedEnemyEvents();
    std::vector<HitEffectEvent> ConsumeHitEffectEvents();

    int GetHP() const { return m_HP; }
    int GetGold() const { return m_Gold; }
    int GetRound() const { return m_Round; }
    int GetTotalRounds() const { return m_TotalRounds; }

    bool IsRoundRunning() const;
    bool IsPaused() const;
    bool IsWin() const;
    bool IsLose() const;
    const char* GetStateName() const;
    bool IsCheatMode() const { return m_CheatMode; }
    void SetCheatMode(bool value) { m_CheatMode = value; }

    DifficultyType GetDifficulty() const { return m_Difficulty; }
    const BuildableRegistry::Entry* GetSelectedBuildableEntry() const { return m_SelectedBuildableEntry; }
    const BuildableRegistry::Entry* GetHoveredBuildableEntry() const { return m_HoveredBuildableEntry; }
    const std::string& GetMessage() const { return m_Message; }
    const std::shared_ptr<IBuildable>& GetSelectedPlacedTower() const { return m_SelectedPlacedTower; }
    int GetSelectedTowerSellRefund() const;
    int GetSelectedTowerUpgradeCost(int upgradePathIndex) const;
    bool CanSelectedTowerUpgrade(int upgradePathIndex) const;
    std::string GetSelectedTowerUpgradeName(int upgradePathIndex) const;

    const MapModel& GetMap() const { return m_Map; }
    const PlacementModel& GetPlacement() const { return m_Placement; }

    const std::vector<std::shared_ptr<IBuildable>>& GetTowers() const { return m_Towers; }
    const std::vector<std::shared_ptr<EnemyModel>>& GetEnemies() const { return m_Enemies; }
    const std::vector<std::shared_ptr<ProjectileModel>>& GetProjectiles() const { return m_Projectiles; }

private:
    struct PlacementCheckResult {
        bool valid = false;
        std::string hintText;
    };

    PlacementCheckResult EvaluatePlacement(
        const std::shared_ptr<IBuildable>& buildable,
        const glm::vec2& position
    ) const;

    void SetupDifficulty();
    void SetupWave();
    void SpawnEnemy();
    void ChangeState(std::unique_ptr<IGameState> state);
    void PauseCurrentState();
    void ResumePausedState();
    void UpdateRoundSpawning(float deltaTimeMs) override;
    void UpdateActiveObjects(float deltaTimeMs) override;
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
    int m_TotalRounds = 50;

    std::unique_ptr<IGameState> m_State;

    const BuildableRegistry::Entry* m_SelectedBuildableEntry = nullptr;
    const BuildableRegistry::Entry* m_HoveredBuildableEntry = nullptr;
    std::string m_Message = "Click tower button (button-0~7) or press 1~7, then click map to place.";
    std::shared_ptr<IBuildable> m_SelectedPlacedTower;

    std::vector<std::shared_ptr<IBuildable>> m_Towers;
    std::vector<std::shared_ptr<EnemyModel>> m_Enemies;
    std::vector<std::shared_ptr<ProjectileModel>> m_Projectiles;

    std::vector<EnemyType> m_CurrentWave;
    int m_SpawnedCount = 0;
    float m_SpawnTimerMs = 0.0f;
    float m_SpawnIntervalMs = 900.0f;
    int m_PoppedBloonCount = 0;
    std::vector<PoppedEnemyEvent> m_PoppedEnemyEvents;
    std::vector<HitEffectEvent> m_HitEffectEvents;
    bool m_CheatMode = false;
};

#endif
