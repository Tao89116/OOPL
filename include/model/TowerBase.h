#ifndef TOWER_BASE_H
#define TOWER_BASE_H

#include "model/IBuildable.h"

class TowerBase : public IBuildable {
public:
    explicit TowerBase(const glm::vec2& position);
    ~TowerBase() override = default;

    std::string GetId() const override { return m_Id; }
    std::string GetDisplayName() const override { return m_DisplayName; }

    int GetCost() const override { return m_Cost; }
    float GetFootprintRadius() const override { return m_FootprintRadius; }
    bool CanPlaceOnPath() const override { return m_CanPlaceOnPath; }

    std::string GetSpriteKey() const override { return m_SpriteKey; }
    std::string GetPreviewSpriteKey() const override { return m_PreviewSpriteKey; }

    bool ShowRangePreview() const override { return m_ShowRangePreview; }
    float GetPreviewRange() const override { return m_PreviewRange; }

    glm::vec2 GetPosition() const override { return m_Position; }
    void SetPosition(const glm::vec2& position) override { m_Position = position; }

    bool ShouldRemove() const override { return m_ShouldRemove; }

protected:
    std::string m_Id;
    std::string m_DisplayName;

    int m_Cost = 0;
    float m_FootprintRadius = 20.0f;
    bool m_CanPlaceOnPath = false;

    std::string m_SpriteKey;
    std::string m_PreviewSpriteKey;

    bool m_ShowRangePreview = false;
    float m_PreviewRange = 0.0f;

    glm::vec2 m_Position = {0.0f, 0.0f};
    bool m_ShouldRemove = false;
};

#endif