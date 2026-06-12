#ifndef START_ROUND_COMMAND_H
#define START_ROUND_COMMAND_H

#include "controller/commands/ICommand.h"

class StartRoundCommand final : public ICommand {
public:
    bool Execute(GameModel& model) override {
        model.StartRound();
        return true;
    }

    bool IsAvailable(const GameModel& model) const override {
        (void)model;
        return true;
    }
};

#endif
