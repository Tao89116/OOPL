//
// Created by polyunicorn on 2026/3/13.
//

//
// Created by polyunicorn on 2026/3/13.
//

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include "pch.hpp"
#include "ResourceManager.h"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "model/GameModel.h"

class UIView {
public:
    explicit UIView(Util::Renderer& renderer);

    void Initialize();
    void Sync(const GameModel& model);

private:
    void InitializeHud();
    void InitializeSelectedInfo();
    void InitializeHelpText();
    void InitializeMessageText();
    void InitializeButtons();
    void InitializeButtonLabels();
    void RegisterObjectsToRenderer();

    void SyncHudText(const GameModel& model);
    void SyncSelectedInfoText(const GameModel& model);
    void SyncMessageText(const GameModel& model);

private:
    Util::Renderer& m_Renderer;
    ResourceManager& m_Resources = ResourceManager::GetInstance();

    bool m_Initialized = false;

    std::shared_ptr<Util::Text> m_HudText = nullptr;
    std::shared_ptr<Util::GameObject> m_HudObject = nullptr;

    std::shared_ptr<Util::Text> m_SelectedText = nullptr;
    std::shared_ptr<Util::GameObject> m_SelectedObject = nullptr;

    std::shared_ptr<Util::Text> m_HelpText = nullptr;
    std::shared_ptr<Util::GameObject> m_HelpObject = nullptr;

    std::shared_ptr<Util::Text> m_MessageText = nullptr;
    std::shared_ptr<Util::GameObject> m_MessageObject = nullptr;

    std::shared_ptr<Util::GameObject> m_Button1 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button2 = nullptr;
    std::shared_ptr<Util::GameObject> m_Button3 = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonStart = nullptr;

    std::shared_ptr<Util::Text> m_Button1Label = nullptr;
    std::shared_ptr<Util::GameObject> m_Button1LabelObj = nullptr;

    std::shared_ptr<Util::Text> m_Button2Label = nullptr;
    std::shared_ptr<Util::GameObject> m_Button2LabelObj = nullptr;

    std::shared_ptr<Util::Text> m_Button3Label = nullptr;
    std::shared_ptr<Util::GameObject> m_Button3LabelObj = nullptr;

    std::shared_ptr<Util::Text> m_ButtonStartLabel = nullptr;
    std::shared_ptr<Util::GameObject> m_ButtonStartLabelObj = nullptr;
};

#endif