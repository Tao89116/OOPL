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
#include "view/UIView.h"

class GameView {
public:
    explicit GameView(DifficultyType difficulty);

    void Initialize(const GameModel& model);
    void Render(const GameModel& model);

private:
    void InitializeStaticObjects(const GameModel& model);
    void SyncTowers(const GameModel& model);
    void SyncEnemies(const GameModel& model);
    void SyncProjectiles(const GameModel& model);
    void SyncPlacementPreview(const GameModel& model);

private:
    DifficultyType m_Difficulty;
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();
    UIView m_UIView;
    bool m_Initialized = false;

    std::shared_ptr<Util::GameObject> m_Background = nullptr;

    std::unordered_map<const TowerBase*, std::shared_ptr<Util::GameObject>> m_TowerObjects;
    std::unordered_map<const EnemyModel*, std::shared_ptr<Util::GameObject>> m_EnemyObjects;
    std::unordered_map<const ProjectileModel*, std::shared_ptr<Util::GameObject>> m_ProjectileObjects;

    std::shared_ptr<Util::GameObject> m_PreviewTowerObject = nullptr;
    std::shared_ptr<Util::GameObject> m_PreviewRangeObject = nullptr;

    std::string m_LastPreviewDefinitionId;
    bool m_LastPreviewValid = true;

    // TODO:
    // 之後若要畫 path debug，可在這裡加 path overlay objects。
};

#endif