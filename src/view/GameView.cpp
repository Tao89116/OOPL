//
// Created by polyunicorn on 2026/3/13.
//

#include "view/GameView.h"

#include "GameConfig.h"
#include "model/AttackTowerBase.h"
#include "Util/Time.hpp"
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
    InitializePopSounds();
    m_UIView.Initialize();
    m_Initialized = true;
}

void GameView::InitializePopSounds() {
    m_PopSounds.clear();
    for (const auto& key : {"pop_01", "pop_02", "pop_03", "pop_04"}) {
        auto sound = std::make_shared<Util::SFX>(m_Resources.GetSoundPath(key));
        sound->SetVolume(96);
        m_PopSounds.push_back(sound);
    }
}

void GameView::QueuePopEffects(const std::vector<GameModel::PoppedEnemyEvent>& events) {
    for (const auto& event : events) {
        if (event.enemy) {
            m_PendingPopEffects.insert(event.enemy);
        }
    }
}

void GameView::QueueHitEffects(const std::vector<HitEffectEvent>& events) {
    for (const auto& event : events) {
        if (!event.soundKey.empty()) {
            auto found = m_HitSounds.find(event.soundKey);
            if (found == m_HitSounds.end()) {
                auto sound = std::make_shared<Util::SFX>(m_Resources.GetSoundPath(event.soundKey));
                sound->SetVolume(96);
                found = m_HitSounds.emplace(event.soundKey, sound).first;
            }
            found->second->Play();
        }

        if (!event.imageKey.empty() && event.durationMs > 0.0f) {
            CreateHitEffect(event);
        }
    }
}

void GameView::PlayPopSounds(int popCount) {
    if (m_PopSounds.empty()) {
        return;
    }

    for (int count = 0; count < popCount; ++count) {
        m_PopSounds[m_NextPopSoundIndex]->Play();
        m_NextPopSoundIndex = (m_NextPopSoundIndex + 1) % m_PopSounds.size();
    }
}

void GameView::InitializeBackground(const GameModel& model) {
    const std::string bgKey = model.GetMap().GetBackgroundKey();
    m_CurrentBackgroundKey = bgKey;

    auto image = m_Resources.GetImage(bgKey);
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};

    m_Renderer.AddChild(m_Background);
}

void GameView::SyncBackground(const GameModel& model) {
    const std::string nextKey = model.GetMap().GetBackgroundKey();
    if (m_Background && nextKey == m_CurrentBackgroundKey) {
        return;
    }

    if (m_Background) {
        m_Renderer.RemoveChild(m_Background);
        m_Background = nullptr;
    }

    InitializeBackground(model);
}

void GameView::Render(const GameModel& model) {
    Initialize(model);
    SyncBackground(model);

    SyncTowerObjects(model);
    SyncEnemyObjects(model);
    SyncProjectileObjects(model);
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();
    SyncPopEffects(deltaTimeMs);
    SyncHitEffects(deltaTimeMs);
    SyncSelectedTowerRangeObject(model);
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
            if (m_PendingPopEffects.erase(it->first) > 0) {
                CreatePopEffectAt(it->second->m_Transform.translation);
            }
            m_Renderer.RemoveChild(it->second);
            it = m_EnemyObjects.erase(it);
        } else {
            ++it;
        }
    }

    m_PendingPopEffects.clear();
}

void GameView::CreatePopEffectAt(const glm::vec2& position) {
    auto obj = std::make_shared<Util::GameObject>(
        m_Resources.GetImage("pop"),
        31.0f
    );

    obj->m_Transform.translation = position;
    obj->m_Transform.scale *= 0.4f;
    m_Renderer.AddChild(obj);

    m_PopEffects.push_back({obj, 180.0f});
}

void GameView::SyncPopEffects(float deltaTimeMs) {
    for (auto it = m_PopEffects.begin(); it != m_PopEffects.end();) {
        it->remainingMs -= deltaTimeMs;
        if (it->remainingMs <= 0.0f) {
            if (it->object) {
                m_Renderer.RemoveChild(it->object);
            }
            it = m_PopEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::CreateHitEffect(const HitEffectEvent& event) {
    auto obj = std::make_shared<Util::GameObject>(
        m_Resources.GetImage(event.imageKey),
        32.0f
    );

    const float effectScale = std::max(event.scale, 0.1f);
    obj->m_Transform.translation = event.position;
    obj->m_Transform.scale = {effectScale, effectScale};
    m_Renderer.AddChild(obj);

    m_HitEffects.push_back({obj, event.durationMs});
}

void GameView::SyncHitEffects(float deltaTimeMs) {
    for (auto it = m_HitEffects.begin(); it != m_HitEffects.end();) {
        it->remainingMs -= deltaTimeMs;
        if (it->remainingMs <= 0.0f) {
            if (it->object) {
                m_Renderer.RemoveChild(it->object);
            }
            it = m_HitEffects.erase(it);
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

void GameView::SyncSelectedTowerRangeObject(const GameModel& model) {
    const auto& selectedTower = model.GetSelectedPlacedTower();
    const auto attackTower = std::dynamic_pointer_cast<AttackTowerBase>(selectedTower);

    if (!attackTower ||
        !attackTower->ShowRangePreview() ||
        attackTower->GetPreviewRange() <= 0.0f) {
        if (m_SelectedTowerRangeObject) {
            m_Renderer.RemoveChild(m_SelectedTowerRangeObject);
            m_SelectedTowerRangeObject = nullptr;
        }
        m_LastSelectedRangeTower = nullptr;
        return;
    }

    const IBuildable* selectedKey = selectedTower.get();
    const std::string rangeKey = "range_circle_valid";

    if (!m_SelectedTowerRangeObject || m_LastSelectedRangeTower != selectedKey) {
        if (m_SelectedTowerRangeObject) {
            m_Renderer.RemoveChild(m_SelectedTowerRangeObject);
        }

        m_SelectedTowerRangeObject = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(rangeKey),
            45.0f
        );
        m_Renderer.AddChild(m_SelectedTowerRangeObject);
        m_LastSelectedRangeTower = selectedKey;
    }

    m_SelectedTowerRangeObject->m_Transform.translation = attackTower->GetPosition();

    auto circleImage = m_Resources.GetImage(rangeKey);
    const float textureRadius = std::max(circleImage->GetSize().x * 0.5f, 1.0f);
    const float scale = attackTower->GetPreviewRange() / textureRadius;
    m_SelectedTowerRangeObject->m_Transform.scale = {scale, scale};
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
}
