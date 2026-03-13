//
// Created by polyunicorn on 2026/3/9.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_GAMESCENE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_GAMESCENE_HPP

#include "pch.hpp"

#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include "ResourceManager.h"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

class GameScene {
public:
    GameScene();

    void Update();

private:
    void InitResources();
    void InitSceneObjects();
    void InitUI();
    void InitGameObjects();
    void UpdateUIText();

private:
    Util::Renderer m_Renderer;
    ResourceManager& m_ResourceManager = ResourceManager::GetInstance();

    std::shared_ptr<Util::GameObject> m_Background = nullptr;

    std::vector<std::shared_ptr<Util::GameObject>> m_Slots;
    std::vector<std::shared_ptr<Tower>> m_Towers;
    std::vector<std::shared_ptr<Enemy>> m_Enemies;
    std::vector<std::shared_ptr<Projectile>> m_Projectiles;

    std::shared_ptr<Util::Text> m_TitleText = nullptr;
    std::shared_ptr<Util::GameObject> m_TitleObject = nullptr;

    std::shared_ptr<Util::Text> m_InfoText = nullptr;
    std::shared_ptr<Util::GameObject> m_InfoObject = nullptr;

    std::vector<glm::vec2> m_SlotPositions;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_GAMESCENE_HPP