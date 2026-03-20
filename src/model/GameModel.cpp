//
// Created by polyunicorn on 2026/3/13.
//

#include "model/GameModel.h"
#include "GameConfig.h"

GameModel::GameModel(DifficultyType difficulty)
    : m_Difficulty(difficulty), m_Map(difficulty) {
    Reset();
}

void GameModel::SetupDifficulty() {
    switch (m_Difficulty) {
        case DifficultyType::Easy:
            m_HP = 30;
            m_Gold = 400;
            m_SpawnIntervalMs = 1100.0f;
            break;
        case DifficultyType::Normal:
            m_HP = 20;
            m_Gold = 300;
            m_SpawnIntervalMs = 900.0f;
            break;
        case DifficultyType::Hard:
            m_HP = 12;
            m_Gold = 220;
            m_SpawnIntervalMs = 720.0f;
            break;
    }
}

void GameModel::Reset() {
    m_Round = 1;
    m_TotalRounds = GameConfig::TotalRounds;
    m_RoundRunning = false;
    m_Paused = false;
    m_Win = false;
    m_Lose = false;

    m_SelectedTowerType = TowerType::Dart;
    m_Message = "Press 1/2/3 to choose tower, click to place.";

    m_Placement.Cancel();

    m_Towers.clear();
    m_Enemies.clear();
    m_Projectiles.clear();

    m_CurrentWave.clear();
    m_SpawnedCount = 0;
    m_SpawnTimerMs = 0.0f;

    SetupDifficulty();
}

void GameModel::SelectTower(TowerType type) {
    m_SelectedTowerType = type;
    BeginPlacement(type);
}

void GameModel::BeginPlacement(TowerType type) {
    if (m_Win || m_Lose) {
        return;
    }

    m_SelectedTowerType = type;
    m_Placement.Begin(type);
    m_Message = "Placing: " + TowerModel::GetDisplayName(type);
}

void GameModel::CancelPlacement() {
    m_Placement.Cancel();
    m_Message = "Placement cancelled.";
}

void GameModel::UpdatePlacementPreview(const glm::vec2& worldPos) {
    if (!m_Placement.IsActive()) {
        return;
    }

    m_Placement.UpdatePreviewPosition(worldPos);
    m_Placement.SetValid(CanPlaceTower(m_Placement.GetTowerType(), worldPos));
}

bool GameModel::CanPlaceTower(TowerType type, const glm::vec2& position) const {
    const int cost = TowerModel::GetCostByType(type);
    if (m_Gold < cost) {
        return false;
    }

    const float radius = TowerModel::GetRadiusByType(type);
    const bool canPlaceOnPath = TowerModel::GetCanPlaceOnPathByType(type);

    // 1. 若該塔不能放在 path 上，就做 path 碰撞判斷
    if (!canPlaceOnPath && m_Map.IsCircleOverlappingAnyPath(position, radius)) {
        return false;
    }

    // 2. 不可和其他塔重疊
    for (const auto& tower : m_Towers) {
        if (!tower) {
            continue;
        }

        const glm::vec2 delta = tower->GetPosition() - position;
        const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        const float minDistance = tower->GetRadius() + radius;

        if (distance < minDistance) {
            return false;
        }
    }

    // TODO: 之後補地圖邊界判斷 / 右側 UI 區域禁放

    return true;
}

bool GameModel::ConfirmPlacement() {
    if (!m_Placement.IsActive()) {
        return false;
    }

    const TowerType type = m_Placement.GetTowerType();
    const glm::vec2 position = m_Placement.GetPreviewPosition();

    if (!CanPlaceTower(type, position)) {
        m_Placement.SetValid(false);
        m_Message = "Cannot place tower here.";
        return false;
    }

    const int cost = TowerModel::GetCostByType(type);
    auto tower = std::make_shared<TowerModel>(type, position);

    m_Towers.push_back(tower);
    m_Gold -= cost;

    // 放完後保留建塔模式，方便連續建造同類塔
    m_Placement.SetValid(false);
    m_Message = "Built " + TowerModel::GetDisplayName(type);

    return true;
}

void GameModel::StartRound() {
    if (m_Win || m_Lose) {
        return;
    }
    if (m_RoundRunning) {
        m_Message = "Round is already running.";
        return;
    }

    SetupWave();
    m_RoundRunning = true;
    m_SpawnedCount = 0;
    m_SpawnTimerMs = 0.0f;
    m_Message = "Round started!";
}

void GameModel::TogglePause() {
    if (m_Win || m_Lose) {
        return;
    }

    m_Paused = !m_Paused;
    m_Message = m_Paused ? "Paused." : "Resume.";
}

void GameModel::SetupWave() {
    m_CurrentWave.clear();

    if (m_Round == 1) {
        m_CurrentWave = {EnemyType::Red, EnemyType::Red, EnemyType::Red, EnemyType::Red, EnemyType::Blue};
    } else if (m_Round == 2) {
        m_CurrentWave = {EnemyType::Red, EnemyType::Blue, EnemyType::Blue, EnemyType::Green, EnemyType::Blue, EnemyType::Red};
    } else if (m_Round == 3) {
        m_CurrentWave = {EnemyType::Blue, EnemyType::Blue, EnemyType::Green, EnemyType::Green, EnemyType::Yellow, EnemyType::Blue};
    } else if (m_Round == 4) {
        m_CurrentWave = {EnemyType::Green, EnemyType::Green, EnemyType::Yellow, EnemyType::Yellow, EnemyType::Blue, EnemyType::Green};
    } else {
        m_CurrentWave = {EnemyType::Yellow, EnemyType::Yellow, EnemyType::Yellow, EnemyType::Green, EnemyType::Green, EnemyType::Blue, EnemyType::Yellow};
    }
}

void GameModel::Update(float deltaTimeMs) {
    if (m_Win || m_Lose || m_Paused) {
        return;
    }

    if (m_RoundRunning) {
        m_SpawnTimerMs += deltaTimeMs;
        if (m_SpawnedCount < static_cast<int>(m_CurrentWave.size()) &&
            m_SpawnTimerMs >= m_SpawnIntervalMs) {
            SpawnEnemy();
            m_SpawnTimerMs = 0.0f;
        }
    }

    UpdateTowers(deltaTimeMs);
    UpdateEnemies(deltaTimeMs);
    UpdateProjectiles(deltaTimeMs);
    CleanupObjects();
    CheckEndState();
}

void GameModel::SpawnEnemy() {
    if (m_SpawnedCount >= static_cast<int>(m_CurrentWave.size())) {
        return;
    }

    const int pathBranchIndex = m_SpawnedCount % static_cast<int>(m_Map.GetPathCount());

    auto enemy = std::make_shared<EnemyModel>(
        m_CurrentWave[m_SpawnedCount],
        m_Map.GetSpawnPoint(pathBranchIndex),
        pathBranchIndex
    );

    m_Enemies.push_back(enemy);
    ++m_SpawnedCount;
}

void GameModel::UpdateTowers(float deltaTimeMs) {
    for (const auto& tower : m_Towers) {
        if (!tower) {
            continue;
        }

        tower->UpdateCooldown(deltaTimeMs);

        if (!tower->CanFire()) {
            continue;
        }

        std::shared_ptr<EnemyModel> target = nullptr;
        float nearestDistance = 999999.0f;

        for (const auto& enemy : m_Enemies) {
            if (!enemy || !enemy->CanBeTargeted()) {
                continue;
            }

            const glm::vec2 delta = enemy->GetPosition() - tower->GetPosition();
            const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            if (distance <= tower->GetRange() && distance < nearestDistance) {
                nearestDistance = distance;
                target = enemy;
            }
        }

        if (target) {
            auto projectile = std::make_shared<ProjectileModel>(
                tower->GetPosition(),
                tower->GetDamage(),
                tower->GetProjectileKey(),
                target
            );
            m_Projectiles.push_back(projectile);
            tower->ResetCooldown();
        }
    }
}

void GameModel::UpdateEnemies(float deltaTimeMs) {
    for (const auto& enemy : m_Enemies) {
        if (enemy) {
            enemy->Update(deltaTimeMs, m_Map.GetPath(enemy->GetPathBranchIndex()));
        }
    }
}

void GameModel::UpdateProjectiles(float deltaTimeMs) {
    for (const auto& projectile : m_Projectiles) {
        if (projectile) {
            projectile->Update(deltaTimeMs);
        }
    }
}

void GameModel::CleanupObjects() {
    for (auto it = m_Enemies.begin(); it != m_Enemies.end();) {
        const auto& enemy = *it;

        if (!enemy) {
            it = m_Enemies.erase(it);
            continue;
        }

        if (!enemy->IsAlive()) {
            if (enemy->HasReachedGoal()) {
                --m_HP;
                m_Message = "A bloon leaked through!";
            } else {
                m_Gold += enemy->GetReward();
            }
            it = m_Enemies.erase(it);
        } else {
            ++it;
        }
    }

    m_Projectiles.erase(
        std::remove_if(
            m_Projectiles.begin(),
            m_Projectiles.end(),
            [](const std::shared_ptr<ProjectileModel>& projectile) {
                return !projectile || !projectile->IsActive();
            }
        ),
        m_Projectiles.end()
    );

    if (m_RoundRunning &&
        m_SpawnedCount >= static_cast<int>(m_CurrentWave.size()) &&
        m_Enemies.empty() &&
        m_Projectiles.empty()) {
        m_RoundRunning = false;

        if (m_Round >= m_TotalRounds) {
            m_Win = true;
            m_Message = "Victory!";
        } else {
            ++m_Round;
            m_Message = "Round cleared. Press SPACE for next round.";
        }
    }
}

void GameModel::CheckEndState() {
    if (m_HP <= 0) {
        m_HP = 0;
        m_Lose = true;
        m_RoundRunning = false;
        m_Message = "Defeat.";
    }
}

void GameModel::ForceWin() {
    if (m_Lose) {
        return;
    }
    m_Win = true;
    m_RoundRunning = false;
    m_Message = "Debug: Force Win";
}

void GameModel::ForceLose() {
    if (m_Win) {
        return;
    }
    m_Lose = true;
    m_RoundRunning = false;
    m_HP = 0;
    m_Message = "Debug: Force Lose";
}