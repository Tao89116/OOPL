//
// Created by polyunicorn on 2026/3/13.
//

//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "pch.hpp"
#include "SceneType.h"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "model/GameModel.h"
#include "model/IBuildable.h"
#include "view/UIView.h"

class GameView {
public:
    explicit GameView(DifficultyType difficulty);

    void Initialize(const GameModel& model);
    void Render(const GameModel& model);

private:
    void InitializeBackground(const GameModel& model);

    void SyncTowerObjects(const GameModel& model);
    void SyncEnemyObjects(const GameModel& model);
    void SyncProjectileObjects(const GameModel& model);

    void SyncPlacementPreviewObjects(const GameModel& model);
    void ClearPreviewObjectsIfInactive(const GameModel& model);
    void CreateOrUpdatePreviewTower(const GameModel& model);
    void CreateOrUpdatePreviewRange(const GameModel& model);

private:
    DifficultyType m_Difficulty;
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();
    UIView m_UIView;
    bool m_Initialized = false;

    std::shared_ptr<Util::GameObject> m_Background = nullptr;

    std::unordered_map<const IBuildable*, std::shared_ptr<Util::GameObject>> m_TowerObjects;
    std::unordered_map<const EnemyModel*, std::shared_ptr<Util::GameObject>> m_EnemyObjects;
    std::unordered_map<const ProjectileModel*, std::shared_ptr<Util::GameObject>> m_ProjectileObjects;

    std::shared_ptr<Util::GameObject> m_PreviewTowerObject = nullptr;
    std::shared_ptr<Util::GameObject> m_PreviewRangeObject = nullptr;

    std::string m_LastPreviewBuildableId;
    bool m_LastPreviewValid = true;
};

#endif