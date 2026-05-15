#include "model/GameModel.h"
#include "GameConfig.h"
#include "model/DifficultyModel.h"
#include <limits>

GameModel::GameModel(DifficultyType difficulty)
    : m_Difficulty(difficulty), m_Map(difficulty) {
    BuildableRegistry::GetInstance().RegisterDefaults();
    Reset();
}
//難易度決定初始資源
//m_message for debug
//interval後期調整
void GameModel::SetupDifficulty() {
    switch (m_Difficulty) {
        case DifficultyType::Easy:
            m_HP = 100;
            m_Gold = 650;
            m_SpawnIntervalMs = 400.0f;
            break;
        case DifficultyType::Normal:
            m_HP = 75;
            m_Gold = 650;
            m_SpawnIntervalMs = 400.0f;
            break;
        case DifficultyType::Hard:
            m_HP = 50;
            m_Gold = 650;
            m_SpawnIntervalMs = 400.0f;
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

    m_SelectedBuildableEntry = BuildableRegistry::GetInstance().FindById("dart_tower");
    m_Message = "Click tower button (button-0~7) or press 1~7, then click map to place.";
    m_SelectedPlacedTower.reset();

    m_Placement.Cancel();

    m_Towers.clear();
    m_Enemies.clear();
    m_Projectiles.clear();

    m_CurrentWave.clear();
    m_SpawnedCount = 0;
    m_SpawnTimerMs = 0.0f;
    m_PoppedBloonCount = 0;
    m_PoppedEnemyEvents.clear();

    SetupDifficulty();
}

void GameModel::SelectBuildable(const BuildableRegistry::Entry* entry) {
    if (!entry) {
        return;
    }

    m_SelectedBuildableEntry = entry;
    BeginPlacement(entry);
}

void GameModel::BeginPlacement(const BuildableRegistry::Entry* entry) {
    if (!entry || m_Win || m_Lose) {
        return;
    }

    m_SelectedBuildableEntry = entry;
    m_Placement.Begin(entry);

    if (m_Placement.GetPreviewBuildable()) {
        m_Message = "Placing: " + m_Placement.GetPreviewBuildable()->GetDisplayName();
    } else {
        m_Message = "Placing...";
    }
}

void GameModel::CancelPlacement() {
    m_Placement.Cancel();
    m_Message = "Placement cancelled.";
}

bool GameModel::SelectPlacedTowerAt(const glm::vec2& worldPos) {
    std::shared_ptr<IBuildable> bestTower;
    float bestDistance = std::numeric_limits<float>::max();

    for (const auto& tower : m_Towers) {
        if (!tower) {
            continue;
        }

        const glm::vec2 delta = tower->GetPosition() - worldPos;
        const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (distance <= tower->GetFootprintRadius() && distance < bestDistance) {
            bestDistance = distance;
            bestTower = tower;
        }
    }

    if (!bestTower) {
        m_SelectedPlacedTower.reset();
        return false;
    }

    m_SelectedPlacedTower = bestTower;
    const int refund = DifficultyModel::GetBuildCost(m_Difficulty, bestTower->GetId()) / 2;
    m_Message = "Selected " + bestTower->GetDisplayName() +
                " (press X to sell, refund " + std::to_string(refund) + ")";
    return true;
}

bool GameModel::SellSelectedTower() {
    if (!m_SelectedPlacedTower) {
        m_Message = "No tower selected. Click a tower first.";
        return false;
    }

    const auto target = m_SelectedPlacedTower;
    const int refund = DifficultyModel::GetBuildCost(m_Difficulty, target->GetId()) / 2;

    const auto eraseBegin = std::remove_if(
        m_Towers.begin(),
        m_Towers.end(),
        [&](const std::shared_ptr<IBuildable>& tower) {
            return tower == target;
        }
    );

    if (eraseBegin == m_Towers.end()) {
        m_SelectedPlacedTower.reset();
        m_Message = "Selected tower no longer exists.";
        return false;
    }

    m_Towers.erase(eraseBegin, m_Towers.end());
    m_Gold += refund;
    m_SelectedPlacedTower.reset();
    m_Message = "Tower sold. +" + std::to_string(refund) + " gold.";
    return true;
}

GameModel::PlacementCheckResult GameModel::EvaluatePlacement(
    const std::shared_ptr<IBuildable>& buildable,
    const glm::vec2& position
) const {
    PlacementCheckResult result;

    if (!buildable) {
        result.valid = false;
        result.hintText = "No buildable selected.";
        return result;
    }

    const int buildCost = DifficultyModel::GetBuildCost(
        m_Difficulty,
        buildable->GetId()
    );

    if (m_Gold < buildCost) {
        result.valid = false;
        result.hintText = "Not enough gold.";
        return result;
    }

    const float halfWidth = GameConfig::WindowWidth * 0.5f;
    const float halfHeight = GameConfig::WindowHeight * 0.5f;

    if (position.x < -halfWidth || position.x > halfWidth ||
        position.y < -halfHeight || position.y > halfHeight) {
        result.valid = false;
        result.hintText = "Out of map bounds.";
        return result;
    }

    const float worldRightPanelX = GameConfig::RightPanelX - GameConfig::CenterX;
    if (position.x >= worldRightPanelX - 70.0f) {
        result.valid = false;
        result.hintText = "Cannot place on UI panel.";
        return result;
    }

    if (!buildable->CanPlaceOnPath() &&
        m_Map.IsCircleOverlappingAnyPath(position, buildable->GetFootprintRadius())) {
        result.valid = false;
        result.hintText = "Blocked by path.";
        return result;
    }

    for (const auto& tower : m_Towers) {
        if (!tower) {
            continue;
        }

        const glm::vec2 delta = tower->GetPosition() - position;
        const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        const float minDistance = tower->GetFootprintRadius() + buildable->GetFootprintRadius();

        if (distance < minDistance) {
            result.valid = false;
            result.hintText = "Too close to another tower.";
            return result;
        }
    }

    result.valid = true;
    result.hintText = "Valid placement.";
    return result;
}

void GameModel::UpdatePlacementPreview(const glm::vec2& worldPos) {
    if (!m_Placement.IsActive()) {
        return;
    }

    m_Placement.UpdatePreviewPosition(worldPos);

    const auto& preview = m_Placement.GetPreviewBuildable();
    const PlacementCheckResult check = EvaluatePlacement(preview, worldPos);
    m_Placement.SetPreviewFeedback(check.valid, check.hintText);
}

bool GameModel::CanPlaceBuildable(
    const std::shared_ptr<IBuildable>& buildable,
    const glm::vec2& position
) const {
    return EvaluatePlacement(buildable, position).valid;
}

bool GameModel::ConfirmPlacement() {
    if (!m_Placement.IsActive()) {
        return false;
    }

    const auto& preview = m_Placement.GetPreviewBuildable();
    const glm::vec2 position = m_Placement.GetPreviewPosition();

    const PlacementCheckResult check = EvaluatePlacement(preview, position);
    m_Placement.SetPreviewFeedback(check.valid, check.hintText);

    if (!check.valid) {
        m_Message = check.hintText;
        return false;
    }

    if (!m_SelectedBuildableEntry) {
        m_Message = "No selected buildable.";
        return false;
    }

    auto buildable = m_SelectedBuildableEntry->factory(position);
    if (!buildable) {
        m_Message = "Failed to build.";
        return false;
    }

    const int buildCost = DifficultyModel::GetBuildCost(
        m_Difficulty,
        buildable->GetId()
    );

    m_Towers.push_back(buildable);
    m_Gold -= buildCost;

    m_Message = "Built " + buildable->GetDisplayName();
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
        m_CurrentWave = {EnemyType::Green, EnemyType::Green, EnemyType::Yellow, EnemyType::Yellow, EnemyType::Black, EnemyType::White};
    } else {
        m_CurrentWave = {EnemyType::Lead, EnemyType::Yellow, EnemyType::Green, EnemyType::Black, EnemyType::White, EnemyType::Rainbow};
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

        tower->Update(deltaTimeMs, m_Enemies, m_Projectiles);
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
            projectile->Update(deltaTimeMs, m_Enemies);
        }
    }
}

int GameModel::ConsumePoppedBloonCount() {
    const int poppedCount = m_PoppedBloonCount;
    m_PoppedBloonCount = 0;
    return poppedCount;
}

std::vector<GameModel::PoppedEnemyEvent> GameModel::ConsumePoppedEnemyEvents() {
    std::vector<PoppedEnemyEvent> events = std::move(m_PoppedEnemyEvents);
    m_PoppedEnemyEvents.clear();
    return events;
}

void GameModel::CleanupObjects() {
    std::vector<std::shared_ptr<EnemyModel>> spawnedChildren;

    for (auto it = m_Enemies.begin(); it != m_Enemies.end();) {
        const auto& enemy = *it;

        if (!enemy) {
            it = m_Enemies.erase(it);
            continue;
        }

        if (!enemy->IsAlive()) {
            const std::optional<EnemyModel::DeathEvent> deathEvent = enemy->ConsumeDeathEvent();
            if (!deathEvent.has_value()) {
                it = m_Enemies.erase(it);
                continue;
            }

            if (deathEvent->reachedGoal) {
                --m_HP;
                m_Message = "A bloon leaked through!";
            } else {
                ++m_PoppedBloonCount;
                m_PoppedEnemyEvents.push_back({enemy.get(), enemy->GetPosition()});
                m_Gold += deathEvent->reward;

                for (EnemyType childType : deathEvent->childrenToSpawn) {
                    auto child = std::make_shared<EnemyModel>(
                        childType,
                        enemy->GetPosition(),
                        enemy->GetPathBranchIndex(),
                        enemy->GetPathIndex()
                    );
                    spawnedChildren.push_back(child);
                }
            }
            it = m_Enemies.erase(it);
        } else {
            ++it;
        }
    }

    if (!spawnedChildren.empty()) {
        m_Enemies.insert(m_Enemies.end(), spawnedChildren.begin(), spawnedChildren.end());
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

    const bool roundEndedThisCleanup =
        m_RoundRunning &&
        m_SpawnedCount >= static_cast<int>(m_CurrentWave.size()) &&
        m_Enemies.empty() &&
        m_Projectiles.empty();

    bool trapsExpiredAtRoundEnd = false;
    if (roundEndedThisCleanup) {
        m_RoundRunning = false;

        for (const auto& tower : m_Towers) {
            if (!tower) {
                continue;
            }
            trapsExpiredAtRoundEnd = tower->OnRoundEnded() || trapsExpiredAtRoundEnd;
        }
    }

    m_Towers.erase(
        std::remove_if(
            m_Towers.begin(),
            m_Towers.end(),
            [this](const std::shared_ptr<IBuildable>& tower) {
                const bool shouldRemove = !tower || tower->ShouldRemove();
                if (shouldRemove && tower == m_SelectedPlacedTower) {
                    m_SelectedPlacedTower.reset();
                }
                return shouldRemove;
            }
        ),
        m_Towers.end()
    );

    if (roundEndedThisCleanup) {
        if (m_Round >= m_TotalRounds) {
            m_Win = true;
            m_Message = trapsExpiredAtRoundEnd ? "Victory! Traps expired." : "Victory!";
        } else {
            ++m_Round;
            m_Message = trapsExpiredAtRoundEnd
                            ? "Round cleared. Traps expired. Press SPACE for next round."
                            : "Round cleared. Press SPACE for next round.";
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
//debugmode
void GameModel::ForceWin() {
    if (m_Lose) {
        return;
    }
    m_Win = true;
    m_RoundRunning = false;
}

void GameModel::ForceLose() {
    if (m_Win) {
        return;
    }
    m_Lose = true;
    m_RoundRunning = false;
    m_HP = 0;
}
