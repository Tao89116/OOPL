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
#include "Util/SFX.hpp"
#include "Util/Text.hpp"
#include "model/ResultModel.h"

class ResultView {
public:
    explicit ResultView(ResultType result);
    void Initialize();
    void Render(const ResultModel& model);
    void PlayResultSoundOnce();

private:
    void InitializeResultImage();
    void InitializeResultSound();
    void RegisterToRenderer();

private:
    ResultType m_Result;

    Util::Renderer m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;
    bool m_ResultSoundPlayed = false;
    std::shared_ptr<Util::GameObject> m_ResultImage = nullptr;
    std::shared_ptr<Util::SFX> m_ResultSound = nullptr;
};

#endif
