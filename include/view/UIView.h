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
#include "view/GameUILayout.h"

class UIView {
public:
    explicit UIView(Util::Renderer& renderer);

    void Initialize();
    void Sync(const GameModel& model);

private:
    void InitializeHud();
    void InitializeMessageText();
    void InitializeButtons();
    void InitializeActionButtons();
    void InitializeHoverTooltip();
    void RegisterObjectsToRenderer();

    void SyncHudText(const GameModel& model);
    void SyncActionButtons(const GameModel& model);
    void SyncHoverTooltip(const GameModel& model);
    std::string BuildTooltipText(const GameModel& model) const;

private:
    Util::Renderer& m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    std::shared_ptr<Util::Text> m_HudText = nullptr;
    std::shared_ptr<Util::GameObject> m_HudObject = nullptr;
    std::shared_ptr<Util::Text> m_BuyItemText = nullptr;
    std::shared_ptr<Util::GameObject> m_BuyItemObject = nullptr;
    std::shared_ptr<Util::Text> m_BuyItemUnderlineText = nullptr;
    std::shared_ptr<Util::GameObject> m_BuyItemUnderlineObject = nullptr;

    std::shared_ptr<Util::Text> m_SelectedText = nullptr;
    std::shared_ptr<Util::GameObject> m_SelectedObject = nullptr;

    std::shared_ptr<Util::Text> m_MessageText = nullptr;
    std::shared_ptr<Util::GameObject> m_MessageObject = nullptr;
    std::shared_ptr<Util::Text> m_HoverTooltipText = nullptr;
    std::shared_ptr<Util::GameObject> m_HoverTooltipObject = nullptr;
    std::string m_LastHoverTooltipText;

    std::array<std::shared_ptr<Util::GameObject>, GameUILayout::TowerButtonCount> m_TowerButtons{};
    std::shared_ptr<Util::GameObject> m_ButtonStart = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonSell = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonUpgrade1 = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonUpgrade2 = nullptr;

    std::shared_ptr<Util::Text> m_ButtonSellText = nullptr;
    std::shared_ptr<Util::Text> m_ButtonUpgrade1Text = nullptr;
    std::shared_ptr<Util::Text> m_ButtonUpgrade2Text = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonSellTextObj = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonUpgrade1TextObj = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonUpgrade2TextObj = nullptr;

    std::shared_ptr<Util::GameObject> m_HudImg = nullptr;

};

#endif
