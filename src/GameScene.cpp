#include "GameScene.h"

#include "Util/Color.hpp"
#include "Util/Time.hpp"

GameScene::GameScene() {
    InitResources();
    InitSceneObjects();
    InitUI();
    InitGameObjects();
    UpdateUIText();
}

void GameScene::InitResources() {
    m_ResourceManager.Initialize();
}

void GameScene::InitSceneObjects() {
    m_Background = std::make_shared<Util::GameObject>(
        m_ResourceManager.GetImage("background"), 0.0F);
    m_Background->m_Transform.translation = {640.0F, 360.0F};
    m_Background->m_Transform.scale = {1.0F, 1.0F};
    m_Renderer.AddChild(m_Background);

    m_SlotPositions = {
        {300.0F, 250.0F},
        {500.0F, 420.0F},
        {750.0F, 260.0F}
    };

    for (const auto& pos : m_SlotPositions) {
        auto slot = std::make_shared<Util::GameObject>(
            m_ResourceManager.GetImage("slot"), 5.0F);
        slot->m_Transform.translation = pos;
        slot->m_Transform.scale = {0.16F, 0.16F};
        m_Slots.push_back(slot);
        m_Renderer.AddChild(slot);
    }
}

void GameScene::InitUI() {
    m_TitleText = m_ResourceManager.CreateText(
        "default",
        28,
        "Bloons Tower Defense - Week 02~03 Prototype",
        Util::Color(255, 255, 255));
    m_TitleObject = std::make_shared<Util::GameObject>(m_TitleText, 100.0F);
    m_TitleObject->m_Transform.translation = {430.0F, 50.0F};

    m_InfoText = m_ResourceManager.CreateText(
        "default",
        20,
        "Loading...",
        Util::Color(255, 220, 120));
    m_InfoObject = std::make_shared<Util::GameObject>(m_InfoText, 100.0F);
    m_InfoObject->m_Transform.translation = {420.0F, 95.0F};

    m_Renderer.AddChild(m_TitleObject);
    m_Renderer.AddChild(m_InfoObject);
}

void GameScene::InitGameObjects() {
    auto tower1 = std::make_shared<Tower>(
        m_ResourceManager.GetImagePath("tower"),
        glm::vec2(300.0F, 250.0F)
    );
    auto tower2 = std::make_shared<Tower>(
        m_ResourceManager.GetImagePath("tower"),
        glm::vec2(500.0F, 420.0F)
    );

    auto enemy1 = std::make_shared<Enemy>(
        m_ResourceManager.GetImagePath("enemy"),
        glm::vec2(120.0F, 360.0F)
    );
    auto enemy2 = std::make_shared<Enemy>(
        m_ResourceManager.GetImagePath("enemy"),
        glm::vec2(200.0F, 360.0F)
    );

    m_Towers.push_back(tower1);
    m_Towers.push_back(tower2);
    m_Enemies.push_back(enemy1);
    m_Enemies.push_back(enemy2);

    m_Renderer.AddChild(tower1);
    m_Renderer.AddChild(tower2);
    m_Renderer.AddChild(enemy1);
    m_Renderer.AddChild(enemy2);
}

void GameScene::UpdateUIText() {
    m_InfoText->SetText(
        "Objects Ready | Towers: " + std::to_string(m_Towers.size()) +
        " | Enemies: " + std::to_string(m_Enemies.size()) +
        " | Projectiles: " + std::to_string(m_Projectiles.size())
    );
}

void GameScene::Update() {
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();

    for (auto& tower : m_Towers) {
        if (tower) {
            tower->Update(deltaTimeMs);
        }
    }

    for (auto& enemy : m_Enemies) {
        if (enemy) {
            enemy->Update(deltaTimeMs);
        }
    }

    for (auto& projectile : m_Projectiles) {
        if (projectile) {
            projectile->Update(deltaTimeMs);
        }
    }

    UpdateUIText();
    m_Renderer.Update();
}