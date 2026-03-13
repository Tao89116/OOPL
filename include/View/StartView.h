//
// Created by polyunicorn on 2026/3/13.
//

#ifndef START_VIEW_H
#define START_VIEW_H

#include "pch.hpp"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

class StartView {
public:
    StartView();
    void Initialize();
    void Render();

private:
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    std::shared_ptr<Util::GameObject> m_Background = nullptr;
    std::shared_ptr<Util::Text> m_TitleText = nullptr;
    std::shared_ptr<Util::GameObject> m_TitleObj = nullptr;
    std::shared_ptr<Util::Text> m_InfoText = nullptr;
    std::shared_ptr<Util::GameObject> m_InfoObj = nullptr;
};

#endif