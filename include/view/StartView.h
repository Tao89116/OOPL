#ifndef START_VIEW_H
#define START_VIEW_H

#include "ResourceManager.h"
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/SFX.hpp"
#include "pch.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class StartModel;

class StartView {
public:
    StartView();
    void Initialize(const StartModel& model);
    void Render(const StartModel& model);
    void PlayPopSounds(int popCount);

private:
    void InitializeBackground();
    void InitializeBananaCat();
    void InitializeBloons(const StartModel& model);
    void InitializePopSounds();
    void RegisterToRenderer();
    void SyncWithModel(const StartModel& model);

private:
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    std::shared_ptr<Util::GameObject> m_Background = nullptr;
    std::shared_ptr<Util::Animation> m_BananaCatAnimation = nullptr;
    std::shared_ptr<Util::GameObject> m_BananaCatObj = nullptr;
    std::vector<std::shared_ptr<Util::GameObject>> m_BloonObjs;
    std::vector<std::shared_ptr<Util::SFX>> m_PopSounds;
    std::size_t m_NextPopSoundIndex = 0;
};

#endif
