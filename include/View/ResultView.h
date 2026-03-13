//
// Created by polyunicorn on 2026/3/13.
//

#ifndef RESULT_VIEW_H
#define RESULT_VIEW_H

#include "pch.hpp"
#include "SceneType.h"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

class ResultView {
public:
    explicit ResultView(ResultType result);
    void Initialize();
    void Render();

private:
    ResultType m_Result;

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