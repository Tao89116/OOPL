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

// void StartView::InitializeTitle() {
//     m_TitleText = m_Resources.CreateText(
//         "default", 36, "Bloons Tower Defense 2", Util::Color(0, 0, 0));
//     m_TitleObj = std::make_shared<Util::GameObject>(m_TitleText, 100.0f);
//     m_TitleObj->m_Transform.translation = {0.0f, 0.0f};
// }

void StartView::InitializeBananaCat() {
    const std::vector<std::string> frameKeys = {
        "banana_cat_01",
        "banana_cat_02",
        "banana_cat_03",
        "banana_cat_04",
        "banana_cat_05",
        "banana_cat_06",
        "banana_cat_07",
    };

    std::vector<std::string> framePaths;
    framePaths.reserve(frameKeys.size());
    for (const auto& key : frameKeys) {
        framePaths.push_back(m_Resources.GetImagePath(key));
    }

    m_BananaCatAnimation = std::make_shared<Util::Animation>(
        framePaths, true, 120, true, 0);
    m_BananaCatObj = std::make_shared<Util::GameObject>(
        m_BananaCatAnimation, 80.0f);
    m_BananaCatObj->m_Transform.translation = {0.0f, 80.0f};
    m_BananaCatObj->m_Transform.scale = {0.75f, 0.75f};
}

void StartView::InitializeInfoText() {
    m_InfoText = m_Resources.CreateText(
        "default", 40,
        "Press ENTER to Start",
        Util::Color(0, 0, 0));
    m_InfoObj = std::make_shared<Util::GameObject>(m_InfoText, 100.0f);
    m_InfoObj->m_Transform.translation = {0.0f, -200.0f};
}

void StartView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    m_Renderer.AddChild(m_BananaCatObj);
    //m_Renderer.AddChild(m_TitleObj);
    m_Renderer.AddChild(m_InfoObj);
}

void StartView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeBananaCat();
    //InitializeTitle();
    InitializeInfoText();
    RegisterToRenderer();

    m_Initialized = true;
}

void StartView::Render() {
    Initialize();
    m_Renderer.Update();
}