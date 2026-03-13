//
// Created by polyunicorn on 2026/3/13.
//
#include "view/UIView.h"

#include "GameConfig.h"
#include "SceneType.h"
#include "Util/Color.hpp"

UIView::UIView(Util::Renderer& renderer)
    : m_Renderer(renderer) {
}

void UIView::Initialize() {
    m_HudText = m_Resources.CreateText("default", 24, "HUD", Util::Color(255, 255, 255));
    m_HudObject = std::make_shared<Util::GameObject>(m_HudText, 100.0f);
    m_HudObject->m_Transform.translation = {260.0f, GameConfig::TopBarY};

    m_SelectedText = m_Resources.CreateText(
        "default",
        20,
        "Selected: Dart Tower\nCost: 100",
        Util::Color(255, 240, 180)
    );
    m_SelectedObject = std::make_shared<Util::GameObject>(m_SelectedText, 101.0f);
    m_SelectedObject->m_Transform.translation = {1085.0f, 520.0f};

    m_HelpText = m_Resources.CreateText(
        "default",
        18,
        "1/2/3 Select Tower | Q/W/E/A/S/D Build | SPACE Start | P Pause | R Reset",
        Util::Color(255, 220, 120)
    );
    m_HelpObject = std::make_shared<Util::GameObject>(m_HelpText, 102.0f);
    m_HelpObject->m_Transform.translation = {420.0f, GameConfig::BottomBarY};

    m_MessageText = m_Resources.CreateText(
        "default",
        20,
        "Press SPACE to start round.",
        Util::Color(255, 180, 180)
    );
    m_MessageObject = std::make_shared<Util::GameObject>(m_MessageText, 103.0f);
    m_MessageObject->m_Transform.translation = {420.0f, 80.0f};

    m_Button1 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_1"), 80.0f);
    m_Button1->m_Transform.translation = {GameConfig::RightPanelX, GameConfig::RightPanelButtonStartY};
    m_Button1->m_Transform.scale = {0.55f, 0.55f};

    m_Button2 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_2"), 80.0f);
    m_Button2->m_Transform.translation = {GameConfig::RightPanelX, GameConfig::RightPanelButtonStartY + GameConfig::RightPanelButtonGap};
    m_Button2->m_Transform.scale = {0.55f, 0.55f};

    m_Button3 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_action"), 80.0f);
    m_Button3->m_Transform.translation = {GameConfig::RightPanelX, GameConfig::RightPanelButtonStartY + GameConfig::RightPanelButtonGap * 2.0f};
    m_Button3->m_Transform.scale = {0.55f, 0.55f};

    m_ButtonStart = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_start"), 80.0f);
    m_ButtonStart->m_Transform.translation = {GameConfig::RightPanelX, GameConfig::RightPanelButtonStartY + GameConfig::RightPanelButtonGap * 3.2f};
    m_ButtonStart->m_Transform.scale = {0.55f, 0.55f};

    m_Button1Label = m_Resources.CreateText("default", 18, "[1] Dart", Util::Color(255, 255, 255));
    m_Button1LabelObj = std::make_shared<Util::GameObject>(m_Button1Label, 81.0f);
    m_Button1LabelObj->m_Transform.translation = {1110.0f, 170.0f};

    m_Button2Label = m_Resources.CreateText("default", 18, "[2] Rapid", Util::Color(255, 255, 255));
    m_Button2LabelObj = std::make_shared<Util::GameObject>(m_Button2Label, 81.0f);
    m_Button2LabelObj->m_Transform.translation = {1110.0f, 265.0f};

    m_Button3Label = m_Resources.CreateText("default", 18, "[3] Heavy", Util::Color(255, 255, 255));
    m_Button3LabelObj = std::make_shared<Util::GameObject>(m_Button3Label, 81.0f);
    m_Button3LabelObj->m_Transform.translation = {1110.0f, 360.0f};

    m_ButtonStartLabel = m_Resources.CreateText("default", 18, "[SPACE] Start", Util::Color(255, 255, 255));
    m_ButtonStartLabelObj = std::make_shared<Util::GameObject>(m_ButtonStartLabel, 81.0f);
    m_ButtonStartLabelObj->m_Transform.translation = {1100.0f, 475.0f};

    m_Renderer.AddChild(m_Button1);
    m_Renderer.AddChild(m_Button2);
    m_Renderer.AddChild(m_Button3);
    m_Renderer.AddChild(m_ButtonStart);

    m_Renderer.AddChild(m_Button1LabelObj);
    m_Renderer.AddChild(m_Button2LabelObj);
    m_Renderer.AddChild(m_Button3LabelObj);
    m_Renderer.AddChild(m_ButtonStartLabelObj);

    m_Renderer.AddChild(m_HudObject);
    m_Renderer.AddChild(m_SelectedObject);
    m_Renderer.AddChild(m_HelpObject);
    m_Renderer.AddChild(m_MessageObject);
}

void UIView::Sync(const GameModel& model) {
    std::string difficultyText = "Easy";
    if (model.GetDifficulty() == DifficultyType::Normal) {
        difficultyText = "Normal";
    } else if (model.GetDifficulty() == DifficultyType::Hard) {
        difficultyText = "Hard";
    }

    m_HudText->SetText(
        "HP: " + std::to_string(model.GetHP()) +
        "    Gold: " + std::to_string(model.GetGold()) +
        "    Round: " + std::to_string(model.GetRound()) + "/" + std::to_string(model.GetTotalRounds()) +
        "    Difficulty: " + difficultyText
    );

    std::string selectedInfo =
        "Selected:\n" +
        TowerModel::GetDisplayName(model.GetSelectedTowerType()) +
        "\nCost: " + std::to_string(TowerModel::GetCostByType(model.GetSelectedTowerType())) +
        "\n\n[Q/W/E/A/S/D]\nBuild on Slot";

    m_SelectedText->SetText(selectedInfo.empty() ? " " : selectedInfo);

    std::string msg = model.GetMessage();
    if (model.IsPaused()) {
        msg = "Paused. Press P to resume.";
    }
    if (msg.empty()) {
        msg = " ";
    }
    m_MessageText->SetText(msg);

    // TODO:
    // 後續 UI 可擴充：
    // - 顯示當前波次剩餘敵人數
    // - 顯示塔詳細數值
    // - 顯示滑鼠 hover 的 slot / tower info
    // - 顯示升級按鈕
}