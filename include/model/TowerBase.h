#ifndef TOWER_BASE_H
#define TOWER_BASE_H

#include "model/IBuildable.h"

class TowerBase : public IBuildable {
public:
    explicit TowerBase(const glm::vec2& position);
    ~TowerBase() override = default;

    std::string GetId() const override { return m_Id; }
    std::string GetDisplayName() const override { return m_DisplayName; }
    std::string GetDescription() const override { return m_Description; }
    std::string GetSpeedText() const override { return m_SpeedText; }

    int GetCost() const override { return m_Cost; }
    float GetFootprintRadius() const override { return m_FootprintRadius; }
    bool CanPlaceOnPath() const override { return m_CanPlaceOnPath; }

    std::string GetSpriteKey() const override { return m_SpriteKey; }
    std::string GetPreviewSpriteKey() const override { return m_PreviewSpriteKey; }

    bool ShowRangePreview() const override { return m_ShowRangePreview; }
    float GetPreviewRange() const override { return m_PreviewRange; }

    glm::vec2 GetPosition() const override { return m_Position; }
    void SetPosition(const glm::vec2& position) override { m_Position = position; }
    float GetRotation() const override { return m_Rotation; }

    bool ShouldRemove() const override { return m_ShouldRemove; }

protected:
    void SetRotation(float rotation) { m_Rotation = rotation; }

protected:
    std::string m_Id;
    std::string m_DisplayName;
    std::string m_Description;
    std::string m_SpeedText = "-";

    int m_Cost = 0;
    float m_FootprintRadius = 20.0f;
    bool m_CanPlaceOnPath = false;

    std::string m_SpriteKey;
    std::string m_PreviewSpriteKey;

    bool m_ShowRangePreview = false;
    float m_PreviewRange = 0.0f;

    glm::vec2 m_Position = {0.0f, 0.0f};
    float m_Rotation = 0.0f;
    bool m_ShouldRemove = false;
};

#endif
