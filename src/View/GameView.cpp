//
// Created by polyunicorn on 2026/3/13.
//

#include "view/GameView.h"
#include "GameConfig.h"
#include <unordered_set>

GameView::GameView(DifficultyType difficulty)
    : m_Difficulty(difficulty), m_UIView(m_Renderer) {
}

void GameView::Initialize(const GameModel& model) {
    if (m_Initialized) {
        return;
    }

    InitializeStaticObjects(model);
    m_UIView.Initialize();
    m_Initialized = true;
}

void GameView::InitializeStaticObjects(const GameModel& model) {
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

    SyncTowers(model);
    SyncEnemies(model);
    SyncProjectiles(model);
    SyncPlacementPreview(model);

    m_UIView.Sync(model);
    m_Renderer.Update();
}

void GameView::SyncTowers(const GameModel& model) {
    std::unordered_set<const TowerBase*> live;

    for (const auto& tower : model.GetTowers()) {
        if (!tower) {
            continue;
        }

        const TowerBase* key = tower.get();
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

void GameView::SyncEnemies(const GameModel& model) {
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

void GameView::SyncProjectiles(const GameModel& model) {
    std::unordered_set<const ProjectileModel*> live;

    for (const auto& projectile : model.GetProjectiles()) {
        if (!projectile) {
            continue;
        }

        const ProjectileModel* key = projectile.get();
        live.insert(key);

        auto found = m_ProjectileObjects.find(key);
        if (found == m_ProjectileObjects.end()) {
            auto obj = std::make_shared<Util::GameObject>(
                m_Resources.GetImage(projectile->GetSpriteKey()),
                40.0f
            );

            obj->m_Transform.scale *= 0.8f;
            m_Renderer.AddChild(obj);
            m_ProjectileObjects[key] = obj;
            found = m_ProjectileObjects.find(key);
        }

        found->second->m_Transform.translation = projectile->GetPosition();
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

void GameView::SyncPlacementPreview(const GameModel& model) {
    const PlacementModel& placement = model.GetPlacement();

    if (!placement.IsActive() || !placement.GetDefinition()) {
        if (m_PreviewTowerObject) {
            m_Renderer.RemoveChild(m_PreviewTowerObject);
            m_PreviewTowerObject = nullptr;
        }
        if (m_PreviewRangeObject) {
            m_Renderer.RemoveChild(m_PreviewRangeObject);
            m_PreviewRangeObject = nullptr;
        }
        m_LastPreviewDefinitionId.clear();
        return;
    }

    const auto& definition = placement.GetDefinition();
    const std::string definitionId = definition->GetId();

    if (!m_PreviewTowerObject || m_LastPreviewDefinitionId != definitionId) {
        if (m_PreviewTowerObject) {
            m_Renderer.RemoveChild(m_PreviewTowerObject);
        }

        auto previewTower = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(definition->GetPreviewSpriteKey()),
            50.0f
        );
        previewTower->m_Transform.scale *= 0.8f;
        m_Renderer.AddChild(previewTower);
        m_PreviewTowerObject = previewTower;
        m_LastPreviewDefinitionId = definitionId;
    }

    m_PreviewTowerObject->m_Transform.translation = placement.GetPreviewPosition();

    if (definition->ShowRangePreview() && definition->GetPreviewRange() > 0.0f) {
        const bool nowValid = placement.IsValid();
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

        m_PreviewRangeObject->m_Transform.translation = placement.GetPreviewPosition();

        auto circleImage = m_Resources.GetImage(rangeKey);
        const float textureRadius = circleImage->GetSize().x * 0.5f;
        const float scale = definition->GetPreviewRange() / textureRadius;
        m_PreviewRangeObject->m_Transform.scale = {scale, scale};
    } else {
        if (m_PreviewRangeObject) {
            m_Renderer.RemoveChild(m_PreviewRangeObject);
            m_PreviewRangeObject = nullptr;
        }
    }
}