#ifndef START_VIEW_H
#define START_VIEW_H

#include "ResourceManager.h"
#include "model/StartModel.h"
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/SFX.hpp"
#include "pch.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class StartView {
public:
    StartView();
    void Initialize(const StartModel& model);
    void Render(const StartModel& model);
    void PlayPopSounds(int popCount);
    void QueuePopEffects(const std::vector<StartModel::PoppedBloonEvent>& events);

private:
    void InitializeBackground();
    void InitializeBananaCat();
    void InitializeBloons(const StartModel& model);
    void InitializePopSounds();
    void RegisterToRenderer();
    void SyncWithModel(const StartModel& model);
    void CreatePopEffectAt(const glm::vec2& position);
    void SyncPopEffects(float deltaTimeMs);

private:
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    struct PopEffect {
        std::shared_ptr<Util::GameObject> object;
        float remainingMs = 0.0f;
    };

    std::shared_ptr<Util::GameObject> m_Background = nullptr;
    std::shared_ptr<Util::Animation> m_BananaCatAnimation = nullptr;
    std::shared_ptr<Util::GameObject> m_BananaCatObj = nullptr;
    std::vector<std::shared_ptr<Util::GameObject>> m_BloonObjs;
    std::vector<PopEffect> m_PopEffects;
    std::vector<std::shared_ptr<Util::SFX>> m_PopSounds;
    std::size_t m_NextPopSoundIndex = 0;
};

#endif
