//
// Created by polyunicorn on 2026/3/20.
//
//changedon326
#include "model/PlacementModel.h"

void PlacementModel::Begin(const std::shared_ptr<IBuildableDefinition>& definition) {
    m_Active = true;
    m_SelectedDefinition = definition;
    m_PreviewPosition = {0.0f, 0.0f};
    m_Valid = false;
    m_HintText = "Move mouse to preview placement.";
}

void PlacementModel::Cancel() {
    m_Active = false;
    m_Valid = false;
    m_SelectedDefinition = nullptr;
    m_HintText.clear();
}

void PlacementModel::UpdatePreviewPosition(const glm::vec2& position) {
    m_PreviewPosition = position;
}

void PlacementModel::SetPreviewFeedback(bool valid, const std::string& hintText) {
    m_Valid = valid;
    m_HintText = hintText;
}