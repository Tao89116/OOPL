//
// Created by polyunicorn on 2026/3/13.
//

//
// Created by polyunicorn on 2026/3/13.
//

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include "pch.hpp"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "model/GameModel.h"

class UIView {
public:
    explicit UIView(Util::Renderer& renderer);

    void Initialize();
    void Sync(const GameModel& model);

private:
    void InitializeHud();
    void InitializeSelectedInfo();
    void InitializeHelpText();
    void InitializeMessageText();
    void InitializeButtons();
    void InitializeButtonLabels();
    void RegisterObjectsToRenderer();

    void SyncHudText(const GameModel& model);
    void SyncSelectedInfoText(const GameModel& model);

private:
    Util::Renderer& m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    std::shared_ptr<Util::Text> m_HudText = nullptr;
    std::shared_ptr<Util::GameObject> m_HudObject = nullptr;

    std::shared_ptr<Util::Text> m_HudBuyItem = nullptr;
    std::shared_ptr<Util::GameObject> m_HudBuyItemObj = nullptr;

    std::shared_ptr<Util::Text> m_SelectedText = nullptr;
    std::shared_ptr<Util::GameObject> m_SelectedObject = nullptr;

    std::shared_ptr<Util::Text> m_HelpText = nullptr;
    std::shared_ptr<Util::GameObject> m_HelpObject = nullptr;

    std::shared_ptr<Util::Text> m_MessageText = nullptr;
    std::shared_ptr<Util::GameObject> m_MessageObject = nullptr;

    std::shared_ptr<Util::GameObject> m_Button1 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button2 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button3 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button4 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button5 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button6 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button7 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button8 = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonStart = nullptr;
    std::shared_ptr<Util::GameObject> m_HudImg = nullptr;

    float verticalPos = 462.5f;
    const float verticalInterval = 45.0f;
    float horizontalGap = 80.0f;
    const float horizontalInterval = 50.0f;
};

#endif
