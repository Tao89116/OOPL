//
// Created by polyunicorn on 2026/3/13.
//

#include "View/UIView.h"

#include "SceneType.h"
#include "Util/Color.hpp"
#include "model/DifficultyModel.h"
#include "model/IBuildable.h"
#include <iostream>

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
    m_HudBuyItem = m_Resources.CreateText(
        "default",
        30,
        "BUY STUFF:",
        Util::Color(255, 255, 255)
    );
    m_HudBuyItemObj = std::make_shared<Util::GameObject>(m_HudBuyItem, 100.0f);
    m_HudBuyItemObj->m_Transform.translation = {540.0f, 235.0f};
    m_HudBuyItemObj->m_Transform.scale *= 1;

}

void UIView::InitializeSelectedInfo() {
    m_SelectedText = m_Resources.CreateText(
        "default",
        16,
        "Selected:\nNone",
        Util::Color(255, 255, 255)
    );
    m_SelectedObject = std::make_shared<Util::GameObject>(m_SelectedText, 100.0f);
    m_SelectedObject->m_Transform.translation = {530.0f, -150.0f};
}

void UIView::InitializeHelpText() {
    m_HelpText = m_Resources.CreateText(
        "default",
        20,
        "X Sell Selected Tower \n P Pause ",
        Util::Color(255, 255, 255)
    );

    if (!m_HelpText) {
        std::cout << "[UI ERROR] m_HelpText create failed\n";
        return;
    }

    m_HelpObject = std::make_shared<Util::GameObject>(m_HelpText, 100.0f);
    m_HelpObject->m_Transform.translation = {530.0f, 0.0f};
}

void UIView::InitializeMessageText() {
    m_MessageText = m_Resources.CreateText("default", 18, "", Util::Color(255, 255, 0));
    m_MessageObject = std::make_shared<Util::GameObject>(m_MessageText, 100.0f);
    m_MessageObject->m_Transform.translation = {0.0f, -330.0f};
}


void UIView::InitializeButtons() {

    m_Button1 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_1"), 80.0f);
    m_Button1->m_Transform.translation = {verticalPos, 2*horizontalInterval+horizontalGap};
    m_Button1->m_Transform.scale *=0.35;

    m_Button2 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_2"), 80.0f);
    m_Button2->m_Transform.translation = {verticalPos + verticalInterval, 2*horizontalInterval+horizontalGap};
    m_Button2->m_Transform.scale *=0.35;

    m_Button3 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_3"), 80.0f);
    m_Button3->m_Transform.translation = {verticalPos + 2 * verticalInterval, 2*horizontalInterval+horizontalGap};
    m_Button3->m_Transform.scale *=0.35;

    m_Button4 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_4"), 80.0f);
    m_Button4->m_Transform.translation = {verticalPos + 3 * verticalInterval, 2*horizontalInterval+horizontalGap};
    m_Button4->m_Transform.scale *=0.35;

    m_Button5 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_5"), 80.0f);
    m_Button5->m_Transform.translation = {verticalPos, 1*horizontalInterval+horizontalGap};
    m_Button5->m_Transform.scale *=0.35;

    m_Button6 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_6"), 80.0f);
    m_Button6->m_Transform.translation = {verticalPos + verticalInterval, 1*horizontalInterval+horizontalGap};
    m_Button6->m_Transform.scale *=0.35;

    m_Button7 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_7"), 80.0f);
    m_Button7->m_Transform.translation = {verticalPos + 2 * verticalInterval, 1*horizontalInterval+horizontalGap};
    m_Button7->m_Transform.scale *=0.35;

    m_Button8 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_8"), 80.0f);
    m_Button8->m_Transform.translation = {verticalPos + 3 * verticalInterval, 1*horizontalInterval + horizontalGap};
    m_Button8->m_Transform.scale *=0.35;

    m_ButtonStart = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_start"), 80.0f);
    m_ButtonStart->m_Transform.translation = {535.0f, -275.0f};
    m_ButtonStart->m_Transform.scale *=0.7;
}


void UIView::RegisterObjectsToRenderer() {
    m_Renderer.AddChild(m_Button1);
    m_Renderer.AddChild(m_Button2);
    m_Renderer.AddChild(m_Button3);
    m_Renderer.AddChild(m_Button4);
    m_Renderer.AddChild(m_Button5);
    m_Renderer.AddChild(m_Button6);
    m_Renderer.AddChild(m_Button7);
    m_Renderer.AddChild(m_Button8);
    m_Renderer.AddChild(m_ButtonStart);

    m_Renderer.AddChild(m_HudObject);
    m_Renderer.AddChild(m_HudBuyItemObj);
    m_Renderer.AddChild(m_SelectedObject);
    m_Renderer.AddChild(m_HelpObject);
    m_Renderer.AddChild(m_HudImg);
    m_Renderer.AddChild(m_MessageObject);
}

void UIView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeHud();
    InitializeSelectedInfo();
    InitializeHelpText();
    InitializeMessageText();
    InitializeButtons();
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

void UIView::SyncSelectedInfoText(const GameModel& model) {
    std::string selectedInfo = "Selected:\nNone";

    std::shared_ptr<IBuildable> preview = model.GetPlacement().GetPreviewBuildable();

    if (!preview) {
        const auto* entry = model.GetSelectedBuildableEntry();
        if (entry) {
            preview = entry->factory({0.0f, 0.0f});
        }
    }

    if (preview) {
        const int buildCost = DifficultyModel::GetBuildCost(
            model.GetDifficulty(),
            preview->GetId()
        );

        selectedInfo =
            "Selected:\n" + preview->GetDisplayName() +
            "\nCost: " + std::to_string(buildCost) +
            "\nRange: " + std::to_string(static_cast<int>(preview->GetPreviewRange())) +
            "\nPath Place: " + std::string(preview->CanPlaceOnPath() ? "Yes" : "No") +
            "\nRange Preview: " + std::string(preview->ShowRangePreview() ? "Yes" : "No");
    }

    m_SelectedText->SetText(selectedInfo.empty() ? " " : selectedInfo);
}


void UIView::Sync(const GameModel& model) {
    Initialize();

    SyncHudText(model);
    SyncSelectedInfoText(model);

}

