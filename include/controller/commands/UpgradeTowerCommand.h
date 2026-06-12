#ifndef UPGRADE_TOWER_COMMAND_H
#define UPGRADE_TOWER_COMMAND_H

#include "controller/commands/ICommand.h"

class UpgradeTowerCommand final : public ICommand {
public:
    explicit UpgradeTowerCommand(int pathIndex) : m_PathIndex(pathIndex) {}

    bool Execute(GameModel& model) override {
        return model.UpgradeSelectedTower(m_PathIndex);
    }

    bool IsAvailable(const GameModel& model) const override {
        const auto& selectedTower = model.GetSelectedPlacedTower();
        return selectedTower &&
               !model.GetPlacement().IsActive() &&
               selectedTower->IsUpgradeable() &&
               model.GetSelectedTowerUpgradeCost(m_PathIndex) < 999999;
    }

private:
    int m_PathIndex;
};

#endif
