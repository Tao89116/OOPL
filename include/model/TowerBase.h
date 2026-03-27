//
// Created by polyunicorn on 2026/3/26.
//

#ifndef TOWER_BASE_H
#define TOWER_BASE_H

#include "pch.hpp"
#include "model/EnemyModel.h"
#include "model/ProjectileModel.h"

class TowerBase {
public:
    explicit TowerBase(const glm::vec2& position);
    virtual ~TowerBase() = default;

    virtual void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) = 0;

    virtual bool ShouldRemove() const { return false; }

    const glm::vec2& GetPosition() const { return m_Position; }
    const std::string& GetSpriteKey() const { return m_SpriteKey; }
    const std::string& GetPreviewSpriteKey() const { return m_PreviewSpriteKey; }
    const std::string& GetDisplayName() const { return m_DisplayName; }

    int GetCost() const { return m_Cost; }
    float GetFootprintRadius() const { return m_FootprintRadius; }
    bool CanPlaceOnPath() const { return m_CanPlaceOnPath; }
    bool ShowRangePreview() const { return m_ShowRangePreview; }

    virtual float GetRange() const { return 0.0f; }

    // 預留升級系統介面
    virtual bool CanUpgrade(int upgradePathIndex) const;
    virtual void Upgrade(int upgradePathIndex);
    virtual int GetUpgradeTier(int upgradePathIndex) const;

protected:
    glm::vec2 m_Position;

    std::string m_DisplayName;
    std::string m_SpriteKey = "tower_basic";
    std::string m_PreviewSpriteKey = "tower_basic";

    int m_Cost = 0;
    float m_FootprintRadius = 20.0f;
    bool m_CanPlaceOnPath = false;
    bool m_ShowRangePreview = true;

    int m_UpgradeTiers[3] = {0, 0, 0};

    // TODO:
    // 之後若要做升級分支、buff、debuff，可從這個基底延伸。
};

#endif