#include "model/PlacementModel.h"

void PlacementModel::Begin(const BuildableRegistry::Entry* entry) {
    m_IsActive = (entry != nullptr);
    m_SelectedEntry = entry;
    m_PreviewBuildable = nullptr;
    m_PreviewPosition = {0.0f, 0.0f};
    m_IsValid = false;
    m_HintText.clear();

    if (m_SelectedEntry) {
        m_PreviewBuildable = m_SelectedEntry->factory(m_PreviewPosition);
    }
}

void PlacementModel::Cancel() {
    m_IsActive = false;
    m_SelectedEntry = nullptr;
    m_PreviewBuildable = nullptr;
    m_PreviewPosition = {0.0f, 0.0f};
    m_IsValid = false;
    m_HintText.clear();
}

void PlacementModel::UpdatePreviewPosition(const glm::vec2& worldPos) {
    if (!m_IsActive || !m_PreviewBuildable) {
        return;
    }

    m_PreviewPosition = worldPos;
    m_PreviewBuildable->SetPosition(worldPos);
}

void PlacementModel::SetPreviewFeedback(bool valid, const std::string& hintText) {
    m_IsValid = valid;
    m_HintText = hintText;
}