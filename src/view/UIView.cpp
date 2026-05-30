//
// Created by polyunicorn on 2026/3/13.
//

#include "View/UIView.h"

#include "Util/Color.hpp"

UIView::UIView(Util::Renderer& renderer)
    : m_Renderer(renderer) {
}

void UIView::InitializeHud() {
    m_HudImg = std::make_shared<Util::GameObject>(m_Resources.GetImage("hud"), 79.0f);
    m_HudImg->m_Transform.translation = {530.0f, 0.0f};
    m_HudImg->m_Transform.scale *= 1;
    m_HudText = m_Resources.CreateText(
        "default",
        26,
        "HUD",
        Util::Color(255, 255, 255)
    );
    m_HudObject = std::make_shared<Util::GameObject>(m_HudText, 100.0f);
    m_HudObject->m_Transform.translation = {505.0f, 295.0f};

    m_BuyItemText = m_Resources.CreateText(
        "default",
        32,
        "BUY ITEM",
        Util::Color(255, 255, 255)
    );
    m_BuyItemObject = std::make_shared<Util::GameObject>(m_BuyItemText, 100.0f);
    m_BuyItemObject->m_Transform.translation = GameUILayout::GetBuyItemLabelPosition();

    m_BuyItemUnderlineText = m_Resources.CreateText(
        "default",
        28,
        "________",
        Util::Color(255, 255, 255)
    );
    m_BuyItemUnderlineObject = std::make_shared<Util::GameObject>(m_BuyItemUnderlineText, 100.0f);
    m_BuyItemUnderlineObject->m_Transform.translation = GameUILayout::GetBuyItemUnderlinePosition();
}

void UIView::InitializeMessageText() {
    m_MessageText = m_Resources.CreateText("default", 18, "", Util::Color(255, 255, 0));
    m_MessageObject = std::make_shared<Util::GameObject>(m_MessageText, 100.0f);
    m_MessageObject->m_Transform.translation = {0.0f, -330.0f};
}


void UIView::InitializeButtons() {
    const auto& towerButtons = GameUILayout::GetTowerButtons();
    for (std::size_t index = 0; index < towerButtons.size(); ++index) {
        const auto& binding = towerButtons[index];
        auto button = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(binding.spriteKey),
            80.0f
        );
        button->m_Transform.translation = binding.hitArea.center;
        button->m_Transform.scale *= binding.renderScale;
        m_TowerButtons[index] = button;
    }

    const auto& startButton = GameUILayout::GetStartRoundButton();
    m_ButtonStart = std::make_shared<Util::GameObject>(
        m_Resources.GetImage(startButton.spriteKey),
        80.0f
    );
    m_ButtonStart->m_Transform.translation = startButton.hitArea.center;
    m_ButtonStart->m_Transform.scale *= startButton.renderScale;
}


void UIView::InitializeActionButtons() {
    const auto& sellButton = GameUILayout::GetSellButton();
    m_ButtonSell = std::make_shared<Util::GameObject>(
        m_Resources.GetImage(sellButton.spriteKey),
        90.0f
    );
    m_ButtonSell->m_Transform.translation = sellButton.hitArea.center;
    m_ButtonSell->m_Transform.scale *= sellButton.renderScale;

    const auto& upgradeButton1 = GameUILayout::GetUpgradeButton(0);
    m_ButtonUpgrade1 = std::make_shared<Util::GameObject>(
        m_Resources.GetImage(upgradeButton1.spriteKey),
        90.0f
    );
    m_ButtonUpgrade1->m_Transform.translation = upgradeButton1.hitArea.center;
    m_ButtonUpgrade1->m_Transform.scale *= upgradeButton1.renderScale;

    const auto& upgradeButton2 = GameUILayout::GetUpgradeButton(1);
    m_ButtonUpgrade2 = std::make_shared<Util::GameObject>(
        m_Resources.GetImage(upgradeButton2.spriteKey),
        90.0f
    );
    m_ButtonUpgrade2->m_Transform.translation = upgradeButton2.hitArea.center;
    m_ButtonUpgrade2->m_Transform.scale *= upgradeButton2.renderScale;

    m_ButtonSellText = m_Resources.CreateText("default", 16, "", Util::Color(255, 255, 255));
    m_ButtonSellTextObj = std::make_shared<Util::GameObject>(m_ButtonSellText, 100.0f);
    m_ButtonSellTextObj->m_Transform.translation = GameUILayout::GetSellPriceLabelPosition();

    m_ButtonUpgrade1Text = m_Resources.CreateText("default", 14, "", Util::Color(255, 255, 255));
    m_ButtonUpgrade1TextObj = std::make_shared<Util::GameObject>(m_ButtonUpgrade1Text, 100.0f);
    m_ButtonUpgrade1TextObj->m_Transform.translation = GameUILayout::GetUpgradePriceLabelPosition(0);

    m_ButtonUpgrade2Text = m_Resources.CreateText("default", 14, "", Util::Color(255, 255, 255));
    m_ButtonUpgrade2TextObj = std::make_shared<Util::GameObject>(m_ButtonUpgrade2Text, 100.0f);
    m_ButtonUpgrade2TextObj->m_Transform.translation = GameUILayout::GetUpgradePriceLabelPosition(1);

    m_SelectedText = m_Resources.CreateText("default", 16, "", Util::Color(255, 255, 255));
    m_SelectedObject = std::make_shared<Util::GameObject>(m_SelectedText, 100.0f);
    m_SelectedObject->m_Transform.translation = GameUILayout::GetSelectedInfoPosition();

    m_ButtonSell->SetVisible(false);
    m_ButtonUpgrade1->SetVisible(false);
    m_ButtonUpgrade2->SetVisible(false);
    m_ButtonSellTextObj->SetVisible(false);
    m_ButtonUpgrade1TextObj->SetVisible(false);
    m_ButtonUpgrade2TextObj->SetVisible(false);
    m_SelectedObject->SetVisible(false);
}

void UIView::RegisterObjectsToRenderer() {
    for (const auto& button : m_TowerButtons) {
        m_Renderer.AddChild(button);
    }
    m_Renderer.AddChild(m_ButtonStart);
    m_Renderer.AddChild(m_ButtonSell);
    m_Renderer.AddChild(m_ButtonUpgrade1);
    m_Renderer.AddChild(m_ButtonUpgrade2);
    m_Renderer.AddChild(m_ButtonSellTextObj);
    m_Renderer.AddChild(m_ButtonUpgrade1TextObj);
    m_Renderer.AddChild(m_ButtonUpgrade2TextObj);

    m_Renderer.AddChild(m_SelectedObject);
    m_Renderer.AddChild(m_HudObject);
    m_Renderer.AddChild(m_BuyItemObject);
    m_Renderer.AddChild(m_BuyItemUnderlineObject);
    m_Renderer.AddChild(m_HudImg);
    m_Renderer.AddChild(m_MessageObject);
}

void UIView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeHud();
    InitializeMessageText();
    InitializeButtons();
    InitializeActionButtons();
    RegisterObjectsToRenderer();

    m_Initialized = true;
}

void UIView::SyncHudText(const GameModel& model) {

    m_HudText->SetText(
        "HP: " + std::to_string(model.GetHP()) +
        "\nGold: " + std::to_string(model.GetGold()) +
        "\nRound: " + std::to_string(model.GetRound()) + "/" + std::to_string(model.GetTotalRounds())
    );
}

void UIView::SyncActionButtons(const GameModel& model) {
    const bool hasSelectedTower = static_cast<bool>(model.GetSelectedPlacedTower());
    const bool showActions = hasSelectedTower && !model.GetPlacement().IsActive();

    m_ButtonSell->SetVisible(showActions);
    m_ButtonSellTextObj->SetVisible(showActions);
    m_SelectedObject->SetVisible(showActions);
    if (showActions) {
        const auto& selectedTower = model.GetSelectedPlacedTower();
        m_ButtonSellText->SetText("$" + std::to_string(model.GetSelectedTowerSellRefund()));
        m_SelectedText->SetText(
            "Selected:\n" + selectedTower->GetDisplayName() +
            "\nRange: " + std::to_string(static_cast<int>(selectedTower->GetPreviewRange())) +
            "\nPath Place: " + std::string(selectedTower->CanPlaceOnPath() ? "Yes" : "No") +
            "\nRange Preview: " + std::string(selectedTower->ShowRangePreview() ? "Yes" : "No")
        );
    }

    const bool showUpgrade1 = showActions && model.GetSelectedPlacedTower()->IsUpgradeable() &&
        model.GetSelectedTowerUpgradeCost(0) < 999999;
    const bool showUpgrade2 = showActions && model.GetSelectedPlacedTower()->IsUpgradeable() &&
        model.GetSelectedTowerUpgradeCost(1) < 999999;

    m_ButtonUpgrade1->SetVisible(showUpgrade1);
    m_ButtonUpgrade1TextObj->SetVisible(showUpgrade1);
    if (showUpgrade1) {
        m_ButtonUpgrade1Text->SetText("$" + std::to_string(model.GetSelectedTowerUpgradeCost(0)));
    }

    m_ButtonUpgrade2->SetVisible(showUpgrade2);
    m_ButtonUpgrade2TextObj->SetVisible(showUpgrade2);
    if (showUpgrade2) {
        m_ButtonUpgrade2Text->SetText("$" + std::to_string(model.GetSelectedTowerUpgradeCost(1)));
    }
}

void UIView::Sync(const GameModel& model) {
    Initialize();

    SyncHudText(model);
    SyncActionButtons(model);

}

