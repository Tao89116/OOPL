#ifndef SELL_TOWER_COMMAND_H
#define SELL_TOWER_COMMAND_H

#include "controller/commands/ICommand.h"

class SellTowerCommand final : public ICommand {
public:
    bool Execute(GameModel& model) override {
        return model.SellSelectedTower();
    }

    bool IsAvailable(const GameModel& model) const override {
        return static_cast<bool>(model.GetSelectedPlacedTower()) && !model.GetPlacement().IsActive();
    }
};

#endif
