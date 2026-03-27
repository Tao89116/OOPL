//
// Created by polyunicorn on 2026/3/13.
//

//
// Created by polyunicorn on 2026/3/13.
//

#include "View/UIView.h"

#include "SceneType.h"
#include "Util/Color.hpp"
#include "model/DifficultyModel.h"
#include "model/IBuildable.h"

UIView::UIView(Util::Renderer& renderer)
    : m_Renderer(renderer) {
}

void UIView::InitializeHud() {
    m_HudText = m_Resources.CreateText(
        "default",
        24,
        "HUD",
        Util::Color(255, 255, 255)
    );
    m_HudObject = std::make_shared<Util::GameObject>(m_HudText, 100.0f);
    m_HudObject->m_Transform.translation = {350.0f, 350.0f};
}

void UIView::InitializeSelectedInfo() {
    m_SelectedText = m_Resources.CreateText(
        "default",
        20,
        "Selected:\nNone",
        Util::Color(255, 240, 180)
    );
    m_SelectedObject = std::make_shared<Util::GameObject>(m_SelectedText, 101.0f);
    m_SelectedObject->m_Transform.translation = {350.0f, 250.0f};
}

void UIView::InitializeHelpText() {
    m_HelpText = m_Resources.CreateText(
        "default",
        18,
        "1/2/3 Select Tower | Left Click Build | Right Click / ESC Cancel | SPACE Start | P Pause | R Reset",
        Util::Color(255, 220, 120)
    );
    m_HelpObject = std::make_shared<Util::GameObject>(m_HelpText, 102.0f);
    m_HelpObject->m_Transform.translation = {-220.0f, -330.0f};
}

void UIView::InitializeMessageText() {
    m_MessageText = m_Resources.CreateText(
        "default",
        20,
        "Press 1/2/3 to choose tower, click to place.",
        Util::Color(255, 180, 180)
    );
    m_MessageObject = std::make_shared<Util::GameObject>(m_MessageText, 103.0f);
    m_MessageObject->m_Transform.translation = {-220.0f, 280.0f};
}

void UIView::InitializeButtons() {
    m_Button1 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_1"), 80.0f);
    m_Button1->m_Transform.translation = {470.0f, 190.0f};
    m_Button1->m_Transform.scale = {0.55f, 0.55f};

    m_Button2 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_2"), 80.0f);
    m_Button2->m_Transform.translation = {470.0f, 95.0f};
    m_Button2->m_Transform.scale = {0.55f, 0.55f};

    m_Button3 = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_3"), 80.0f);
    m_Button3->m_Transform.translation = {470.0f, 0.0f};
    m_Button3->m_Transform.scale = {0.55f, 0.55f};

    m_ButtonStart = std::make_shared<Util::GameObject>(m_Resources.GetImage("btn_tower_4"), 80.0f);
    m_ButtonStart->m_Transform.translation = {460.0f, -115.0f};
    m_ButtonStart->m_Transform.scale = {0.55f, 0.55f};
}

void UIView::InitializeButtonLabels() {
    m_Button1Label = m_Resources.CreateText(
        "default",
        18,
        "[1] Dart",
        Util::Color(255, 255, 255)
    );
    m_Button1LabelObj = std::make_shared<Util::GameObject>(m_Button1Label, 81.0f);
    m_Button1LabelObj->m_Transform.translation = {470.0f, 35.0f};

    m_Button2Label = m_Resources.CreateText(
        "default",
        18,
        "[2] Track",
        Util::Color(255, 255, 255)
    );
    m_Button2LabelObj = std::make_shared<Util::GameObject>(m_Button2Label, 81.0f);
    m_Button2LabelObj->m_Transform.translation = {470.0f, 130.0f};

    m_Button3Label = m_Resources.CreateText(
        "default",
        18,
        "[3] IceBall",
        Util::Color(255, 255, 255)
    );
    m_Button3LabelObj = std::make_shared<Util::GameObject>(m_Button3Label, 81.0f);
    m_Button3LabelObj->m_Transform.translation = {470.0f, 225.0f};

    m_ButtonStartLabel = m_Resources.CreateText(
        "default",
        18,
        "[SPACE] Start",
        Util::Color(255, 255, 255)
    );
    m_ButtonStartLabelObj = std::make_shared<Util::GameObject>(m_ButtonStartLabel, 81.0f);
    m_ButtonStartLabelObj->m_Transform.translation = {470.0f, 320.0f};
}

void UIView::RegisterObjectsToRenderer() {
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

void UIView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeHud();
    InitializeSelectedInfo();
    InitializeHelpText();
    InitializeMessageText();
    InitializeButtons();
    InitializeButtonLabels();
    RegisterObjectsToRenderer();

    m_Initialized = true;
}

void UIView::SyncHudText(const GameModel& model) {
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

void UIView::SyncMessageText(const GameModel& model) {
    std::string msg = model.GetMessage();

    const auto& preview = model.GetPlacement().GetPreviewBuildable();
    if (model.GetPlacement().IsActive() && preview) {
        msg = "Placing: " + preview->GetDisplayName() +
              " | " + model.GetPlacement().GetHintText();
    }

    if (model.IsPaused()) {
        msg = "Paused. Press P to resume.";
    }

    if (msg.empty()) {
        msg = " ";
    }

    m_MessageText->SetText(msg);
}

void UIView::Sync(const GameModel& model) {
    Initialize();

    SyncHudText(model);
    SyncSelectedInfoText(model);
    SyncMessageText(model);

    // TODO:
    // 後續 UI 可擴充：
    // - SyncUpgradePanel()
    // - SyncTrapInfo()
    // - SyncWavePreview()
}