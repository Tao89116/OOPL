//
// Created by polyunicorn on 2026/3/13.
//

#include "View/ResultView.h"

#include "GameConfig.h"
#include "Util/Color.hpp"

ResultView::ResultView(ResultType result)
    : m_Result(result) {
}

void ResultView::InitializeBackground() {
    auto image = m_Resources.GetImage("bg_result");
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};
}

void ResultView::InitializeTitle() {
    const std::string title = (m_Result == ResultType::Win) ? "YOU WIN!" : "GAME OVER";
    const Util::Color color = (m_Result == ResultType::Win)
        ? Util::Color(150, 255, 150)
        : Util::Color(255, 150, 150);

    m_TitleText = m_Resources.CreateText("default", 40, title, color);
    m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
    m_TitleObj->m_Transform.translation = {0.0f, 0.0f};
}

void ResultView::InitializeInfoText() {
    m_InfoText = m_Resources.CreateText(
        "default", 24,
        "Press ENTER to return to Start",
        Util::Color(255, 240, 180));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 100.0f);
    m_InfoObj->m_Transform.translation = {450.0f, 300.0f};
}

void ResultView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_TitleObj);
    m_Renderer.AddChild(m_InfoObj);
}

void ResultView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeTitle();
    InitializeInfoText();
    RegisterToRenderer();

    m_Initialized = true;
}

void ResultView::Render() {
    Initialize();
    m_Renderer.Update();
}