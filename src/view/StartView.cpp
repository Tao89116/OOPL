#include "View/StartView.h"

#include "GameConfig.h"
#include "Util/Color.hpp"

StartView::StartView() = default;

void StartView::InitializeBackground() {
    auto image = m_Resources.GetImage("bg_start");
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};
}

void StartView::InitializeTitle() {
    m_TitleText = m_Resources.CreateText(
        "default", 36, "Bloons Tower Defense Prototype", Util::Color(0, 0, 0));
    m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
    m_TitleObj->m_Transform.translation = {420.0f, 120.0f};
}

void StartView::InitializeInfoText() {
    m_InfoText = m_Resources.CreateText(
        "default", 24,
        "Press ENTER to Start\nPress ESC to Exit",
        Util::Color(255, 240, 180));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 101.0f);
    m_InfoObj->m_Transform.translation = {500.0f, 260.0f};
}

void StartView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_TitleObj);
    m_Renderer.AddChild(m_InfoObj);
}

void StartView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeTitle();
    InitializeInfoText();
    RegisterToRenderer();

    m_Initialized = true;
}

void StartView::Render() {
    Initialize();
    m_Renderer.Update();
}