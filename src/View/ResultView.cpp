//
// Created by polyunicorn on 2026/3/13.
//
#include "view/ResultView.h"

#include "GameConfig.h"
#include "Util/Color.hpp"

ResultView::ResultView(ResultType result)
    : m_Result(result) {
}

void ResultView::Initialize() {
    if (m_Initialized) {
        return;
    }

    m_Background = std::make_shared<Util::GameObject>(m_Resources.GetImage("bg_result"), 0.0f);
    auto image = m_Resources.GetImage("bg_result");

    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    float scaleX = GameConfig::WindowWidth  / image->GetSize().x;
    float scaleY = GameConfig::WindowHeight / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0, 0.0};
    m_Background->m_Transform.scale = {scaleX, scaleY};

    std::string title = (m_Result == ResultType::Win) ? "YOU WIN!" : "GAME OVER";
    Util::Color color = (m_Result == ResultType::Win)
        ? Util::Color(150, 255, 150)
        : Util::Color(255, 150, 150);

    m_TitleText = m_Resources.CreateText("default", 40, title, color);
    m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
    m_TitleObj->m_Transform.translation = {560.0f, 180.0f};

    m_InfoText = m_Resources.CreateText(
        "default", 24,
        "Press ENTER to return to Start",
        Util::Color(255, 240, 180));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 101.0f);
    m_InfoObj->m_Transform.translation = {450.0f, 300.0f};

    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_TitleObj);
    m_Renderer.AddChild(m_InfoObj);

    m_Initialized = true;
}

void ResultView::Render() {
    Initialize();
    m_Renderer.Update();
}