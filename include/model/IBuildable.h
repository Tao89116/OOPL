//
// Created by ASUS on 2026/3/27.
//

#ifndef IBUILDABLE_H
#define IBUILDABLE_H

#include "pch.hpp"

class EnemyModel;
class ProjectileModel;

class IBuildable {
public:
    virtual ~IBuildable() = default;

    virtual void Update(
        float deltaTimeMs,
        std::vector<std::shared_ptr<EnemyModel>>& enemies,
        std::vector<std::shared_ptr<ProjectileModel>>& projectiles
    ) = 0;

    // 基本資料
    virtual std::string GetId() const = 0;
    virtual std::string GetDisplayName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual std::string GetSpeedText() const = 0;
    virtual std::string GetUpgradeSummary() const;

    virtual int GetCost() const = 0;
    virtual float GetFootprintRadius() const = 0;
    virtual bool CanPlaceOnPath() const = 0;

    // UI / Preview
    virtual std::string GetSpriteKey() const = 0;
    virtual std::string GetPreviewSpriteKey() const = 0;
    virtual bool ShowRangePreview() const = 0;
    virtual float GetPreviewRange() const = 0;

    // Transform
    virtual glm::vec2 GetPosition() const = 0;
    virtual void SetPosition(const glm::vec2& position) = 0;
    virtual float GetRotation() const = 0;

    // Lifecycle
    virtual bool ShouldRemove() const = 0;
    virtual bool OnRoundEnded() { return false; }

    // Upgrade hooks. Traps keep the default non-upgradeable behavior.
    virtual bool IsUpgradeable() const { return false; }
    virtual int GetUpgradeTier(int pathIndex) const { (void)pathIndex; return 0; }
    virtual std::string GetUpgradeName(int pathIndex) const { (void)pathIndex; return ""; }
    virtual bool ApplyUpgrade(int pathIndex) { (void)pathIndex; return false; }
};

inline std::string IBuildable::GetUpgradeSummary() const {
    if (!IsUpgradeable()) {
        return "";
    }

    std::string summary;
    for (int pathIndex = 0; pathIndex < 2; ++pathIndex) {
        const std::string upgradeName = GetUpgradeName(pathIndex);
        if (upgradeName.empty()) {
            continue;
        }

        if (!summary.empty()) {
            summary += " and\n";
        }
        summary += upgradeName;
    }

    return summary.empty() ? "" : "Can upgrade to\n" + summary;
}

#endif
