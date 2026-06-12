#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "model/GameModel.h"

class SceneManager;

class ICommand {
public:
    virtual ~ICommand() = default;

    virtual bool Execute(GameModel& model) = 0;

    virtual bool Execute(GameModel& model, SceneManager* sceneManager) {
        (void)sceneManager;
        return Execute(model);
    }

    virtual bool IsAvailable(const GameModel& model) const = 0;
};

#endif
