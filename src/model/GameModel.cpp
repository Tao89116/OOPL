//
// Created by polyunicorn on 2026/3/13.
//
#include "model/GameModel.h"

#include "GameConfig.h"

#include <algorithm>
#include <cmath>

GameModel::GameModel(DifficultyType difficulty)
    : m_Difficulty(difficulty) {
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

    // TODO:
    // 之後不同難度可不只影響 HP / Gold / SpawnInterval，
    // 還可以影響：
    // - 敵人種類
    // - 波數
    // - 回合獎勵
    // - 地圖路徑
    // - 塔位數量
}

void GameModel::Reset() {
    m_Round = 1;
    m_TotalRounds = GameConfig::TotalRounds;

    m_RoundRunning = false;
    m_Paused = false;
    m_Win = false;
    m_Lose = false;

    m_SelectedTowerType = TowerType::Dart;
    m_Message = "Press SPACE to start round.";

    m_Towers.clear();
    m_Enemies.clear();
    m_Projectiles.clear();

    m_SlotOccupied.assign(m_Map.GetTowerSlots().size(), false);

    m_CurrentWave.clear();
    m_SpawnedCount = 0;
    m_SpawnTimerMs = 0.0f;

    SetupDifficulty();
}

void GameModel::SelectTower(TowerType type) {
    m_SelectedTowerType = type;
    m_Message = "Selected: " + TowerModel::GetDisplayName(type);
}

bool GameModel::PlaceTowerAtSlot(int slotIndex) {
    if (m_Win || m_Lose) {
        return false;
    }

    if (slotIndex < 0 || slotIndex >= static_cast<int>(m_SlotOccupied.size())) {
        m_Message = "Invalid slot.";
        return false;
    }

    if (m_SlotOccupied[slotIndex]) {
        m_Message = "That slot is already occupied.";
        return false;
    }

    const int cost = TowerModel::GetCostByType(m_SelectedTowerType);
    if (m_Gold < cost) {
        m_Message = "Not enough gold.";
        return false;
    }

    auto tower = std::make_shared<TowerModel>(
        m_SelectedTowerType,
        m_Map.GetTowerSlots()[slotIndex]
    );

    m_Towers.push_back(tower);
    m_SlotOccupied[slotIndex] = true;
    m_Gold -= cost;
    m_Message = "Built " + TowerModel::GetDisplayName(m_SelectedTowerType);

    // TODO:
    // 後續這裡可以加入：
    // - 建塔音效
    // - 建塔動畫
    // - 建塔失敗特效 / 提示
    // - 滑鼠點擊 slot 建塔
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
        m_CurrentWave = {
            EnemyType::Red, EnemyType::Red, EnemyType::Red,
            EnemyType::Red, EnemyType::Blue
        };
    } else if (m_Round == 2) {
        m_CurrentWave = {
            EnemyType::Red, EnemyType::Blue, EnemyType::Blue,
            EnemyType::Green, EnemyType::Blue, EnemyType::Red
        };
    } else if (m_Round == 3) {
        m_CurrentWave = {
            EnemyType::Blue, EnemyType::Blue, EnemyType::Green,
            EnemyType::Green, EnemyType::Yellow, EnemyType::Blue
        };
    } else if (m_Round == 4) {
        m_CurrentWave = {
            EnemyType::Green, EnemyType::Green, EnemyType::Yellow,
            EnemyType::Yellow, EnemyType::Blue, EnemyType::Green
        };
    } else {
        m_CurrentWave = {
            EnemyType::Yellow, EnemyType::Yellow, EnemyType::Yellow,
            EnemyType::Green, EnemyType::Green, EnemyType::Blue,
            EnemyType::Yellow
        };
    }

    // TODO:
    // 後續可把 wave data 抽成外部設定：
    // - json
    // - csv
    // - txt
    // 這樣調整平衡會更方便
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

    auto enemy = std::make_shared<EnemyModel>(
        m_CurrentWave[m_SpawnedCount],
        m_Map.GetSpawnPoint()
    );

    m_Enemies.push_back(enemy);
    ++m_SpawnedCount;

    // TODO:
    // 之後可以在這裡補：
    // - 敵人生成特效
    // - 生成音效
    // - 更複雜的敵人分裂／多層氣球機制
}

void GameModel::UpdateTowers(float deltaTimeMs) {
    for (const auto& tower : m_Towers) {
        if (!tower) {
            continue;
        }

        tower->UpdateCooldown(deltaTimeMs);

        // TODO:
        // 目前先保留基本開火邏輯。
        // 後續要補：
        // - 不同 tower 的 targeting priority
        // - first / last / strong / close
        // - 升級系統
        // - 範圍傷害 / 減速 / 爆炸效果

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
            enemy->Update(deltaTimeMs, m_Map.GetPathPoints());
        }
    }

    // TODO:
    // 之後若要做 Bloons 更接近原作的行為，可以補：
    // - 氣球被打爆後分裂
    // - 特殊抗性
    // - 加速區段
    // - 飛行單位 / 裝甲單位
}

void GameModel::UpdateProjectiles(float deltaTimeMs) {
    for (const auto& projectile : m_Projectiles) {
        if (projectile) {
            projectile->Update(deltaTimeMs);
        }
    }

    // TODO:
    // 之後這裡可以加入：
    // - 命中特效物件
    // - 子彈生命週期
    // - 穿透
    // - 爆炸半徑
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

    // TODO:
    // 若未來加入基地血條、額外生命、護盾等系統，
    // 可在這裡集中處理勝敗判定
}
//cheatmode for test
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