#ifndef PLACEMENT_MODEL_H
#define PLACEMENT_MODEL_H

#include "pch.hpp"
#include "model/IBuildable.h"
#include "model/BuildableRegistry.h"

class PlacementModel {
public:
    void Begin(const BuildableRegistry::Entry* entry);
    void Cancel();

    bool IsActive() const { return m_IsActive; }

    const BuildableRegistry::Entry* GetSelectedEntry() const { return m_SelectedEntry; }
    const std::shared_ptr<IBuildable>& GetPreviewBuildable() const { return m_PreviewBuildable; }

    void UpdatePreviewPosition(const glm::vec2& worldPos);
    glm::vec2 GetPreviewPosition() const { return m_PreviewPosition; }

    void SetPreviewFeedback(bool valid, const std::string& hintText);
    bool IsValid() const { return m_IsValid; }
    const std::string& GetHintText() const { return m_HintText; }

private:
    bool m_IsActive = false;
    const BuildableRegistry::Entry* m_SelectedEntry = nullptr;
    std::shared_ptr<IBuildable> m_PreviewBuildable = nullptr;

    glm::vec2 m_PreviewPosition = {0.0f, 0.0f};
    bool m_IsValid = false;
    std::string m_HintText;
};

#endif