#include "view/StartView.h"

#include "GameConfig.h"
#include "model/StartModel.h"
#include "Util/Time.hpp"

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

namespace {

const glm::vec2 kPlayButtonCenter = {540.0f, -285.0f};
const glm::vec2 kPlayButtonHalfSize = {50.0f, 28.0f};
constexpr float kPlayButtonScale = 1.5f;

bool IsInsideRect(const glm::vec2& point, const glm::vec2& center, const glm::vec2& halfSize) {
    return point.x >= center.x - halfSize.x && point.x <= center.x + halfSize.x &&
           point.y >= center.y - halfSize.y && point.y <= center.y + halfSize.y;
}

} // namespace

void StartView::InitializeBloons(const StartModel& model) {
    m_BloonObjs.clear();
    for (const auto& bloon : model.GetBloons()) {
        auto bloonImage = m_Resources.GetImage(bloon.spriteKey);
        auto bloonObj = std::make_shared<Util::GameObject>(bloonImage, 60.0f);
        const glm::vec2 imageSize = bloonImage->GetSize();
        bloonObj->m_Transform.translation = bloon.position;
        bloonObj->m_Transform.scale = {
            GameConfig::StartBloonWidth / imageSize.x,
            GameConfig::StartBloonHeight / imageSize.y,
        };
        bloonObj->SetVisible(!bloon.popped);
        m_BloonObjs.push_back(bloonObj);
    }
}

void StartView::InitializePlayButton() {
    m_PlayButton = std::make_shared<Util::GameObject>(
        m_Resources.GetImage("btn_play"),
        90.0f
    );
    m_PlayButton->m_Transform.translation = kPlayButtonCenter;
    m_PlayButton->m_Transform.scale *= kPlayButtonScale;
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
    m_Renderer.AddChild(m_PlayButton);
}

void StartView::Initialize(const StartModel& model) {
    if (m_Initialized) {
        return;
    }

    InitializeBackground();
    InitializeBloons(model);
    InitializeBananaCat();
    InitializePlayButton();
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
    const float catScaleX = model.IsBananaCatFacingRight() ? -0.6f : 0.6f;
    m_BananaCatObj->m_Transform.scale = {catScaleX, 0.6f};
}

void StartView::Render(const StartModel& model) {
    Initialize(model);
    SyncWithModel(model);
    SyncPopEffects(Util::Time::GetDeltaTimeMs());
    m_Renderer.Update();
}

void StartView::QueuePopEffects(const std::vector<StartModel::PoppedBloonEvent>& events) {
    for (const auto& event : events) {
        CreatePopEffectAt(event.position);
    }
}

void StartView::CreatePopEffectAt(const glm::vec2& position) {
    auto obj = std::make_shared<Util::GameObject>(
        m_Resources.GetImage("pop"),
        61.0f
    );

    obj->m_Transform.translation = position;
    obj->m_Transform.scale *= 0.4f;
    m_Renderer.AddChild(obj);

    m_PopEffects.push_back({obj, 180.0f});
}

void StartView::SyncPopEffects(float deltaTimeMs) {
    for (auto it = m_PopEffects.begin(); it != m_PopEffects.end();) {
        it->remainingMs -= deltaTimeMs;
        if (it->remainingMs <= 0.0f) {
            if (it->object) {
                m_Renderer.RemoveChild(it->object);
            }
            it = m_PopEffects.erase(it);
        } else {
            ++it;
        }
    }
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

bool StartView::IsPlayButtonHit(const glm::vec2& point) const {
    return IsInsideRect(point, kPlayButtonCenter, kPlayButtonHalfSize);
}
