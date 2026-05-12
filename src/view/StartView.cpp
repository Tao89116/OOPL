#include "view/StartView.h"

#include "GameConfig.h"
#include "model/StartModel.h"

StartView::StartView() = default;

void StartView::InitializeBackground() {
    auto image = m_Resources.GetImage("bg_start");
    m_Background = std::make_shared<Util::GameObject>(image, 0.0f);

    const float scaleX = static_cast<float>(GameConfig::WindowWidth) / image->GetSize().x;
    const float scaleY = static_cast<float>(GameConfig::WindowHeight) / image->GetSize().y;

    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {scaleX, scaleY};
}

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
    m_BananaCatObj->m_Transform.scale = {0.75f, 0.75f};
}

void StartView::InitializeBloons(const StartModel& model) {
    m_BloonObjs.clear();
    for (const auto& bloon : model.GetBloons()) {
        auto bloonObj = std::make_shared<Util::GameObject>(
            m_Resources.GetImage(bloon.spriteKey), 60.0f);
        bloonObj->m_Transform.translation = bloon.position;
        bloonObj->m_Transform.scale = {
            GameConfig::StartBloonScale,
            GameConfig::StartBloonScale,
        };
        bloonObj->SetVisible(!bloon.popped);
        m_BloonObjs.push_back(bloonObj);
    }
}

void StartView::InitializePopSounds() {
    m_PopSounds.clear();
    for (const auto& key : {"pop_01", "pop_02", "pop_03", "pop_04"}) {
        auto sound = std::make_shared<Util::SFX>(m_Resources.GetSoundPath(key));
        sound->SetVolume(96);
        m_PopSounds.push_back(sound);
    }
}

void StartView::RegisterToRenderer() {
    m_Renderer.AddChild(m_Background);
    for (const auto& bloonObj : m_BloonObjs) {
        m_Renderer.AddChild(bloonObj);
    }
    m_Renderer.AddChild(m_BananaCatObj);
}

void StartView::Initialize(const StartModel& model) {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeBloons(model);
    InitializeBananaCat();
    InitializePopSounds();
    SyncWithModel(model);
    RegisterToRenderer();

    m_Initialized = true;
}

void StartView::SyncWithModel(const StartModel& model) {
    const auto& bloons = model.GetBloons();
    for (std::size_t index = 0; index < bloons.size() && index < m_BloonObjs.size(); ++index) {
        m_BloonObjs[index]->m_Transform.translation = bloons[index].position;
        m_BloonObjs[index]->SetVisible(!bloons[index].popped);
    }

    m_BananaCatObj->m_Transform.translation = model.GetBananaCatPosition();
    const float catScaleX = model.IsBananaCatFacingRight() ? 0.75f : -0.75f;
    m_BananaCatObj->m_Transform.scale = {catScaleX, 0.75f};
}

void StartView::Render(const StartModel& model) {
    Initialize(model);
    SyncWithModel(model);
    m_Renderer.Update();
}

void StartView::PlayPopSounds(int popCount) {
    if (m_PopSounds.empty()) {
        return;
    }

    for (int count = 0; count < popCount; ++count) {
        m_PopSounds[m_NextPopSoundIndex]->Play();
        m_NextPopSoundIndex = (m_NextPopSoundIndex + 1) % m_PopSounds.size();
    }
}
