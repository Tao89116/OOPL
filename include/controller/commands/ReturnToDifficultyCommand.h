#ifndef RETURN_TO_DIFFICULTY_COMMAND_H
#define RETURN_TO_DIFFICULTY_COMMAND_H

#include "controller/commands/ICommand.h"
#include "SceneType.h"
#include "scene/SceneManager.h"

class ReturnToDifficultyCommand final : public ICommand {
public:
    bool Execute(GameModel& model) override {
        (void)model;
        return false;
    }

    bool Execute(GameModel& model, SceneManager* sceneManager) override {
        (void)model;
        if (sceneManager == nullptr) {
            return false;
        }

        sceneManager->SetGameSession(nullptr);
        sceneManager->RequestSceneChange(SceneType::Difficulty);
        return true;
    }

    bool IsAvailable(const GameModel& model) const override {
        (void)model;
        return true;
    }
};

#endif
