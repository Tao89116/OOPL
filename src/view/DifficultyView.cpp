//
// Created by polyunicorn on 2026/3/13.
//

#include "View/DifficultyView.h"

#include "GameConfig.h"
#include "Util/Color.hpp"

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
        "default", 34, "Select Difficulty", Util::Color(255, 255, 255));
    m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
    m_TitleObj->m_Transform.translation = {500.0f, 120.0f};
}

void DifficultyView::InitializeInfoText() {
    m_InfoText = m_Resources.CreateText(
        "default", 24,
        "[1] Easy\n[2] Normal\n[3] Hard\n\nPress BACKSPACE to return",
        Util::Color(255, 240, 180));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 101.0f);
    m_InfoObj->m_Transform.translation = {540.0f, 240.0f};
}

void DifficultyView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_TitleObj);
    m_Renderer.AddChild(m_InfoObj);
}

void DifficultyView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeTitle();
    InitializeInfoText();
    RegisterToRenderer();

    m_Initialized = true;
}

void DifficultyView::Render() {
    Initialize();
    m_Renderer.Update();
}