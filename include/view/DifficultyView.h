//
// Created by polyunicorn on 2026/3/13.
//

#ifndef DIFFICULTY_VIEW_H
#define DIFFICULTY_VIEW_H

#include "pch.hpp"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include <vector>


class DifficultyView {
public:
    DifficultyView();
    void Initialize();
    void Render();

private:
    struct DifficultyCardObjects {
        std::shared_ptr<Util::GameObject> preview;
        std::shared_ptr<Util::GameObject> phrame;
        std::shared_ptr<Util::Text> labelText;
        std::shared_ptr<Util::GameObject> label;
    };

    void InitializeBackground();
    void InitializeTitle();
    void InitializeInfoText();
    void InitializeDifficultyCards();
    void RegisterToRenderer();

private:
    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;
    std::shared_ptr<Util::GameObject> m_Background = nullptr;
    std::shared_ptr<Util::Text> m_TitleText = nullptr;
    std::shared_ptr<Util::GameObject> m_TitleObj = nullptr;
    std::shared_ptr<Util::Text> m_InfoText = nullptr;
    std::shared_ptr<Util::GameObject> m_InfoObj = nullptr;
    std::vector<DifficultyCardObjects> m_DifficultyCards;
};

#endif
