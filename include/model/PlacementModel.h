//
// Created by polyunicorn on 2026/3/20.
//
//changedon326
#ifndef PLACEMENT_MODEL_H
#define PLACEMENT_MODEL_H

#include "pch.hpp"
#include "model/IBuildableDefinition.h"

class PlacementModel {
public:
    void Begin(const std::shared_ptr<IBuildableDefinition>& definition);
    void Cancel();
    void UpdatePreviewPosition(const glm::vec2& position);
    void SetPreviewFeedback(bool valid, const std::string& hintText);

    bool IsActive() const { return m_Active; }
    bool IsValid() const { return m_Valid; }
    const std::shared_ptr<IBuildableDefinition>& GetDefinition() const { return m_SelectedDefinition; }
    const glm::vec2& GetPreviewPosition() const { return m_PreviewPosition; }
    const std::string& GetHintText() const { return m_HintText; }

private:
    bool m_Active = false;
    bool m_Valid = false;
    std::shared_ptr<IBuildableDefinition> m_SelectedDefinition = nullptr;
    glm::vec2 m_PreviewPosition = {0.0f, 0.0f};
    std::string m_HintText;

    // TODO: 之後若要支援拖曳建塔、旋轉塔、或建塔 UI 選單，
    // 可在這裡加 selected card / rotation / hovered panel state。
};

#endif