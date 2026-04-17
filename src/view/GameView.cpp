//
// Created by polyunicorn on 2026/3/13.
//

#include "view/GameView.h"

#include "GameConfig.h"
#include <algorithm>
#include <unordered_set>

GameView::GameView(DifficultyType difficulty)
    : m_Difficulty(difficulty), m_UIView(m_Renderer) {
}

void GameView::Initialize(const GameModel& model) {
    if (m_Initialized) {
        return;
    }

    InitializeBackground(model);
    m_UIView.Initialize();
    m_Initialized = true;
}

void GameView::InitializeBackground(const GameModel& model) {
    const std::string bgKey = model.GetMap().GetBackgroundKey();

    auto image = m_Resources.GetImage(bgKey);
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};

    m_Renderer.AddChild(m_Background);
}

void GameView::Render(const GameModel& model) {
    Initialize(model);

    SyncTowerObjects(model);
    SyncEnemyObjects(model);
    SyncProjectileObjects(model);
    SyncPlacementPreviewObjects(model);

    m_UIView.Sync(model);
    m_Renderer.Update();
}

void GameView::SyncTowerObjects(const GameModel& model) {
    std::unordered_set<const IBuildable*> live;

    for (const auto& tower : model.GetTowers()) {
        if (!tower) {
            continue;
        }

        const IBuildable* key = tower.get();
        live.insert(key);

        auto found = m_TowerObjects.find(key);
        if (found == m_TowerObjects.end()) {
            auto obj = std::make_shared<Util::GameObject>(
                m_Resources.GetImage(tower->GetSpriteKey()),
                20.0f
            );

            obj->m_Transform.scale *= 0.8f;
            m_Renderer.AddChild(obj);
            m_TowerObjects[key] = obj;
            found = m_TowerObjects.find(key);
        }

        found->second->m_Transform.translation = tower->GetPosition();
        found->second->m_Transform.rotation = tower->GetRotation();
    }

    for (auto it = m_TowerObjects.begin(); it != m_TowerObjects.end();) {
        if (live.find(it->first) == live.end()) {
            m_Renderer.RemoveChild(it->second);
            it = m_TowerObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::SyncEnemyObjects(const GameModel& model) {
    std::unordered_set<const EnemyModel*> live;

    for (const auto& enemy : model.GetEnemies()) {
        if (!enemy) {
            continue;
        }

        const EnemyModel* key = enemy.get();
        live.insert(key);

        auto found = m_EnemyObjects.find(key);
        if (found == m_EnemyObjects.end()) {
            auto obj = std::make_shared<Util::GameObject>(
                m_Resources.GetImage(enemy->GetSpriteKey()),
                30.0f
            );

            obj->m_Transform.scale *= 0.6f;
            m_Renderer.AddChild(obj);
            m_EnemyObjects[key] = obj;
            found = m_EnemyObjects.find(key);
        }

        found->second->m_Transform.translation = enemy->GetPosition();
    }

    for (auto it = m_EnemyObjects.begin(); it != m_EnemyObjects.end();) {
        if (live.find(it->first) == live.end()) {
            m_Renderer.RemoveChild(it->second);
            it = m_EnemyObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::SyncProjectileObjects(const GameModel& model) {
    std::unordered_set<const ProjectileModel*> live;

    for (const auto& projectile : model.GetProjectiles()) {
        if (!projectile) {
            continue;
        }

        const ProjectileModel* key = projectile.get();
        live.insert(key);

        auto found = m_ProjectileObjects.find(key);
        if (found == m_ProjectileObjects.end()) {
            auto projectileImage = m_Resources.GetImage(projectile->GetSpriteKey());
            auto obj = std::make_shared<Util::GameObject>(
                projectileImage,
                40.0f
            );

            obj->m_Transform.scale *= 0.8f;
            obj->SetPivot(projectileImage->GetSize() * 0.5f);
            m_Renderer.AddChild(obj);
            m_ProjectileObjects[key] = obj;
            found = m_ProjectileObjects.find(key);
        }

        found->second->m_Transform.translation = projectile->GetPosition();
        found->second->m_Transform.rotation = projectile->GetRotation();
        float projectileScale = 0.8f * projectile->GetRenderScale();
        const float visualRadius = projectile->GetVisualRadius();
        if (visualRadius >= 0.0f) {
            auto projectileImage = m_Resources.GetImage(projectile->GetSpriteKey());
            const float textureRadius = std::max(projectileImage->GetSize().x * 0.5f, 1.0f);
            projectileScale = visualRadius / textureRadius;
        }
        found->second->m_Transform.scale = {projectileScale, projectileScale};
    }

    for (auto it = m_ProjectileObjects.begin(); it != m_ProjectileObjects.end();) {
        if (live.find(it->first) == live.end()) {
            m_Renderer.RemoveChild(it->second);
            it = m_ProjectileObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::SyncPlacementPreviewObjects(const GameModel& model) {
    ClearPreviewObjectsIfInactive(model);

    if (!model.GetPlacement().IsActive() || !model.GetPlacement().GetPreviewBuildable()) {
        return;
    }

    CreateOrUpdatePreviewTower(model);
    CreateOrUpdatePreviewRange(model);
}

void GameView::ClearPreviewObjectsIfInactive(const GameModel& model) {
    const auto& preview = model.GetPlacement().GetPreviewBuildable();

    if (model.GetPlacement().IsActive() && preview) {
        return;
    }

    if (m_PreviewTowerObject) {
        m_Renderer.RemoveChild(m_PreviewTowerObject);
        m_PreviewTowerObject = nullptr;
    }

    if (m_PreviewRangeObject) {
        m_Renderer.RemoveChild(m_PreviewRangeObject);
        m_PreviewRangeObject = nullptr;
    }

    m_LastPreviewBuildableId.clear();
}

void GameView::CreateOrUpdatePreviewTower(const GameModel& model) {
    const auto& preview = model.GetPlacement().GetPreviewBuildable();
    const std::string buildableId = preview->GetId();

    if (!m_PreviewTowerObject || m_LastPreviewBuildableId != buildableId) {
        if (m_PreviewTowerObject) {
            m_Renderer.RemoveChild(m_PreviewTowerObject);
        }

        auto previewTower = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(preview->GetPreviewSpriteKey()),
            100.0f
        );
        previewTower->m_Transform.scale *= 0.8f;
        m_Renderer.AddChild(previewTower);

        m_PreviewTowerObject = previewTower;
        m_LastPreviewBuildableId = buildableId;
    }

    m_PreviewTowerObject->m_Transform.translation = preview->GetPosition();
}

void GameView::CreateOrUpdatePreviewRange(const GameModel& model) {
    const auto& preview = model.GetPlacement().GetPreviewBuildable();

    if (!preview->ShowRangePreview() || preview->GetPreviewRange() <= 0.0f) {
        if (m_PreviewRangeObject) {
            m_Renderer.RemoveChild(m_PreviewRangeObject);
            m_PreviewRangeObject = nullptr;
        }
        return;
    }

    const bool nowValid = model.GetPlacement().IsValid();
    const std::string rangeKey = nowValid ? "range_circle_valid" : "range_circle_invalid";

    if (!m_PreviewRangeObject || nowValid != m_LastPreviewValid) {
        if (m_PreviewRangeObject) {
            m_Renderer.RemoveChild(m_PreviewRangeObject);
        }

        m_PreviewRangeObject = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(rangeKey),
            45.0f
        );
        m_Renderer.AddChild(m_PreviewRangeObject);
        m_LastPreviewValid = nowValid;
    }

    m_PreviewRangeObject->m_Transform.translation = preview->GetPosition();

    auto circleImage = m_Resources.GetImage(rangeKey);
    const float textureRadius = circleImage->GetSize().x * 0.5f;
    const float scale = preview->GetPreviewRange() / textureRadius;
    m_PreviewRangeObject->m_Transform.scale = {scale, scale};

    // TODO:
    // 其他 preview：
    // Cannon 爆炸半徑
    // Trap 觸發範圍
    // Support Tower buff 範圍
    // 可能?依 buildable type 顯示不同 preview
}
