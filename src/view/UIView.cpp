//
// Created by polyunicorn on 2026/3/13.
//

#include "view/UIView.h"

#include "Util/Color.hpp"
#include "model/DifficultyModel.h"

namespace {

constexpr std::size_t kHoverTooltipMaxLineLength = 20;

void AppendWrappedWord(std::string& wrappedText, std::string& currentLine, const std::string& word) {
    if (word.empty()) {
        return;
    }

    if (word.size() > kHoverTooltipMaxLineLength) {
        if (!currentLine.empty()) {
            wrappedText += currentLine + "\n";
            currentLine.clear();
        }

        for (std::size_t index = 0; index < word.size(); index += kHoverTooltipMaxLineLength) {
            const std::string chunk = word.substr(index, kHoverTooltipMaxLineLength);
            if (index + kHoverTooltipMaxLineLength < word.size()) {
                wrappedText += chunk + "\n";
            } else {
                currentLine = chunk;
            }
        }
        return;
    }

    if (currentLine.empty()) {
        currentLine = word;
        return;
    }

    if (currentLine.size() + 1 + word.size() <= kHoverTooltipMaxLineLength) {
        currentLine += " " + word;
        return;
    }

    wrappedText += currentLine + "\n";
    currentLine = word;
}

std::string WrapTooltipLine(const std::string& line) {
    std::string wrappedText;
    std::string currentLine;

    std::size_t wordStart = 0;
    while (wordStart < line.size()) {
        while (wordStart < line.size() && line[wordStart] == ' ') {
            ++wordStart;
        }

        const std::size_t wordEnd = line.find(' ', wordStart);
        const std::string word = line.substr(
            wordStart,
            wordEnd == std::string::npos ? std::string::npos : wordEnd - wordStart
        );
        AppendWrappedWord(wrappedText, currentLine, word);

        if (wordEnd == std::string::npos) {
            break;
        }
        wordStart = wordEnd + 1;
    }

    if (!currentLine.empty()) {
        wrappedText += currentLine;
    }

    return wrappedText;
}

std::string WrapTooltipText(const std::string& text) {
    std::string wrappedText;
    std::size_t lineStart = 0;

    while (lineStart <= text.size()) {
        const std::size_t lineEnd = text.find('\n', lineStart);
        const std::string line = text.substr(
            lineStart,
            lineEnd == std::string::npos ? std::string::npos : lineEnd - lineStart
        );

        if (!wrappedText.empty()) {
            wrappedText += "\n";
        }
        wrappedText += WrapTooltipLine(line);

        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
    }

    return wrappedText;
}

} // namespace

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
        "_____________",
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

void UIView::InitializeHoverTooltip() {
    m_HoverTooltipBackgroundObject = std::make_shared<Util::GameObject>(
        m_Resources.GetImage("whiteground"),
        99.0f
    );
    m_HoverTooltipBackgroundObject->m_Transform.translation = {530.0f, -40.0f};
    m_HoverTooltipBackgroundObject->SetVisible(false);

    m_HoverTooltipText = m_Resources.CreateText("default", 16, "", Util::Color(20, 20, 20));
    m_HoverTooltipObject = std::make_shared<Util::GameObject>(m_HoverTooltipText, 100.0f);
    m_HoverTooltipObject->m_Transform.translation = {530.0f, 5.0f};
    m_HoverTooltipObject->SetVisible(false);
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

    m_SelectedText = m_Resources.CreateText("default", 20, "", Util::Color(255, 255, 255));
    m_SelectedObject = std::make_shared<Util::GameObject>(m_SelectedText, 98.0f);
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
    m_Renderer.AddChild(m_HoverTooltipBackgroundObject);
    m_Renderer.AddChild(m_HoverTooltipObject);
}

void UIView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeHud();
    InitializeMessageText();
    InitializeHoverTooltip();
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

std::string UIView::BuildTooltipText(const GameModel& model) const {
    const auto* entry = model.GetHoveredBuildableEntry();
    if (!entry) {
        return "";
    }

    const auto buildable = entry->factory({0.0f, 0.0f});
    if (!buildable) {
        return "";
    }

    std::string text = buildable->GetDisplayName() +
        "\nCost: " + std::to_string(DifficultyModel::GetBuildCost(model.GetDifficulty(), buildable->GetId())) +
        "\nSpeed: " + buildable->GetSpeedText() +
        "\n" + buildable->GetDescription();

    const std::string upgradeSummary = buildable->GetUpgradeSummary();
    if (!upgradeSummary.empty()) {
        text += "\n" + upgradeSummary;
    }

    return WrapTooltipText(text);
}

void UIView::SyncHoverTooltip(const GameModel& model) {
    const std::string tooltipText = BuildTooltipText(model);
    const bool showTooltip = !tooltipText.empty();

    m_HoverTooltipBackgroundObject->SetVisible(showTooltip);
    m_HoverTooltipObject->SetVisible(showTooltip);
    if (!showTooltip || tooltipText == m_LastHoverTooltipText) {
        return;
    }

    m_LastHoverTooltipText = tooltipText;
    m_HoverTooltipText->SetText(tooltipText);
}

void UIView::Sync(const GameModel& model) {
    Initialize();

    SyncHudText(model);
    SyncActionButtons(model);
    SyncHoverTooltip(model);

}

