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
    std::string bgKey = "bg_easy";
    if (m_Difficulty == DifficultyType::Normal) {
        bgKey = "bg_normal";
    } else if (m_Difficulty == DifficultyType::Hard) {
        bgKey = "bg_hard";
    }

    //m_Background = std::make_shared<Util::GameObject>(m_Resources.GetImage(bgKey), 0.0f);
    auto image = m_Resources.GetImage(bgKey);

    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    float scaleX = GameConfig::WindowWidth  / image->GetSize().x;
    float scaleY = GameConfig::WindowHeight / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0, 0.0};
    m_Background->m_Transform.scale = {scaleX, scaleY};
    m_Renderer.AddChild(m_Background);

    for (const auto& slotPos : model.GetMap().GetTowerSlots()) {
        auto slot = std::make_shared<Util::GameObject>(m_Resources.GetImage("tower_slot"), 10.0f);
        slot->m_Transform.translation = slotPos;
        slot->m_Transform.scale = {0.18f, 0.18f};
        m_SlotObjects.push_back(slot);
        m_Renderer.AddChild(slot);
    }
}

void GameView::Render(const GameModel& model) {
    Initialize(model);

    SyncTowers(model);
    SyncEnemies(model);
    SyncProjectiles(model);
    m_UIView.Sync(model);

    m_Renderer.Update();
}

void GameView::SyncTowers(const GameModel& model) {
    std::unordered_set<const TowerModel*> live;

    for (const auto& tower : model.GetTowers()) {
        if (!tower) {
            continue;
        }

        const TowerModel* key = tower.get();
        live.insert(key);

        auto found = m_TowerObjects.find(key);
        if (found == m_TowerObjects.end()) {
            auto obj = std::make_shared<Util::GameObject>(
                m_Resources.GetImage(tower->GetSpriteKey()),
                20.0f
            );
            //obj->m_Transform.scale = {0.20f, 0.20f};
            obj->m_Transform.scale*=0.8;
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
            //obj->m_Transform.scale = {0.13f, 0.13f};
            obj->m_Transform.scale*=0.8f;
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
            //obj->m_Transform.scale = {0.10f, 0.10f};
            obj->m_Transform.scale*=0.8f;
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

    // TODO:
    // 後續可以補：
    // - 命中特效物件池
    // - 飛行方向旋轉
    // - 爆炸動畫
}