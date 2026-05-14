//
// Created by polyunicorn on 2026/3/13.
//

#include "view/DifficultyView.h"

#include "GameConfig.h"
#include "model/DifficultyModel.h"
#include "Util/Color.hpp"

namespace {
constexpr glm::vec2 kPhrameDisplaySize = {260.0f, 260.0f};
constexpr glm::vec2 kPreviewDisplaySize = {200.0f, 150.0f};
constexpr float kLabelYOffset = -165.0f;
}

DifficultyView::DifficultyView() = default;

void DifficultyView::InitializeBackground() {
    auto image = m_Resources.GetImage("bg_start");
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};
}

void DifficultyView::InitializeTitle() {
    m_TitleText = m_Resources.CreateText(
        "default", 40, "Select Difficulty", Util::Color(255, 255, 255));
    m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
    m_TitleObj->m_Transform.translation = {0.0f, 285.0f};
}

void DifficultyView::InitializeInfoText() {
    m_InfoText = m_Resources.CreateText(
        "default", 20,
        "Click a phrame to start  |  BACKSPACE to return",
        Util::Color(255, 240, 180));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 100.0f);
    m_InfoObj->m_Transform.translation = {0.0f, -280.0f};
}

void DifficultyView::InitializeDifficultyCards() {
    m_DifficultyCards.clear();
    const auto phrameImage = m_Resources.GetImage("difficulty_phrame");
    const glm::vec2 phrameScale = {
        kPhrameDisplaySize.x / phrameImage->GetSize().x,
        kPhrameDisplaySize.y / phrameImage->GetSize().y,
    };

    for (const auto& option : DifficultyModel::GetDifficultyOptions()) {
        DifficultyCardObjects card;

        const auto previewImage = m_Resources.GetImage(option.backgroundImageKey);
        card.preview = std::make_shared<Util::GameObject>(previewImage, 20.0f);
        card.preview->m_Transform.translation = option.cardCenter;
        card.preview->m_Transform.scale = {
            kPreviewDisplaySize.x / previewImage->GetSize().x,
            kPreviewDisplaySize.y / previewImage->GetSize().y,
        };

        card.phrame = std::make_shared<Util::GameObject>(phrameImage, 30.0f);
        card.phrame->m_Transform.translation = option.cardCenter;
        card.phrame->m_Transform.scale = phrameScale;

        card.labelText = m_Resources.CreateText(
            "default", 28, option.label, Util::Color(255, 255, 255));
        card.label = std::make_shared<Util::GameObject>(card.labelText, 40.0f);
        card.label->m_Transform.translation = {
            option.cardCenter.x,
            option.cardCenter.y + kLabelYOffset,
        };

        m_DifficultyCards.push_back(card);
    }
}

void DifficultyView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_TitleObj);
    for (const auto& card : m_DifficultyCards) {
        m_Renderer.AddChild(card.preview);
        m_Renderer.AddChild(card.phrame);
        m_Renderer.AddChild(card.label);
    }
    m_Renderer.AddChild(m_InfoObj);
}

void DifficultyView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeTitle();
    InitializeInfoText();
    InitializeDifficultyCards();
    RegisterToRenderer();

    m_Initialized = true;
}

void DifficultyView::Render() {
    Initialize();
    m_Renderer.Update();
}
