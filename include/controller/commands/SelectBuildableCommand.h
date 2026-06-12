#ifndef SELECT_BUILDABLE_COMMAND_H
#define SELECT_BUILDABLE_COMMAND_H

#include "controller/commands/ICommand.h"
#include <string>
#include <utility>

class SelectBuildableCommand final : public ICommand {
public:
    explicit SelectBuildableCommand(std::string buildableId)
        : m_BuildableId(std::move(buildableId)) {}

    bool Execute(GameModel& model) override {
        if (m_BuildableId.empty()) {
            return false;
        }

        model.SelectBuildable(BuildableRegistry::GetInstance().FindById(m_BuildableId));
        return true;
    }

    bool IsAvailable(const GameModel& model) const override {
        (void)model;
        return !m_BuildableId.empty();
    }

private:
    std::string m_BuildableId;
};

#endif
