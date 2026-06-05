//
// Created by polyunicorn on 2026/3/13.
//

#include "View/ResultView.h"

#include "GameConfig.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float kStartYOffset = -260.0f;
constexpr float kFinalYOffset = 0.0f;
constexpr float kMaxDisplayWidth = 900.0f;
constexpr float kMaxDisplayHeight = 420.0f;

std::string GetResultImageKey(ResultType result) {
    return (result == ResultType::Win) ? "result_win" : "result_gameover";
}

std::string GetResultSoundKey(ResultType result) {
    return (result == ResultType::Win) ? "result_win" : "result_gameover";
}

float EaseOutCubic(float value) {
    const float clamped = std::clamp(value, 0.0f, 1.0f);
    return 1.0f - std::pow(1.0f - clamped, 3.0f);
}
}

ResultView::ResultView(ResultType result)
    : m_Result(result) {
}

void ResultView::InitializeResultImage() {
    auto image = m_Resources.GetImage(GetResultImageKey(m_Result));
    m_ResultImage = std::make_shared<Util::GameObject>(image, 120.0f);

    const float scaleX = kMaxDisplayWidth / std::max(image->GetSize().x, 1.0f);
    const float scaleY = kMaxDisplayHeight / std::max(image->GetSize().y, 1.0f);
    const float imageScale = std::min({scaleX, scaleY, 1.0f});

    m_ResultImage->m_Transform.translation = {0.0f, kStartYOffset};
    m_ResultImage->m_Transform.scale = {imageScale, imageScale};
}

void ResultView::InitializeResultSound() {
    m_ResultSound = std::make_shared<Util::SFX>(m_Resources.GetSoundPath(GetResultSoundKey(m_Result)));
    m_ResultSound->SetVolume(112);
}

void ResultView::RegisterToRenderer() {
    m_Renderer.AddChild(m_ResultImage);
}

void ResultView::Initialize() {
    if (m_Initialized) {
        return;
    }

    InitializeResultImage();
    InitializeResultSound();
    RegisterToRenderer();

    m_Initialized = true;
}

void ResultView::PlayResultSoundOnce() {
    Initialize();
    if (!m_ResultSound || m_ResultSoundPlayed) {
        return;
    }

    m_ResultSound->Play();
    m_ResultSoundPlayed = true;
}

void ResultView::Render(const ResultModel& model) {
    Initialize();

    if (m_ResultImage) {
        const float easedProgress = EaseOutCubic(model.GetEnterProgress());
        const float y = kStartYOffset + (kFinalYOffset - kStartYOffset) * easedProgress;
        m_ResultImage->m_Transform.translation = {0.0f, y};
    }

    m_Renderer.Update();
}
