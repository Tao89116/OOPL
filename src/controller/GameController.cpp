#include "controller/GameController.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "view/GameUILayout.h"
#include <array>

namespace {

bool IsActionAvailable(const GameUILayout::ButtonBinding& binding, const GameModel& model) {
    const bool hasSelectedTower = static_cast<bool>(model.GetSelectedPlacedTower());
    const bool showActions = hasSelectedTower && !model.GetPlacement().IsActive();

    switch (binding.command) {
    case GameUILayout::CommandType::SellSelectedTower:
        return showActions;
    case GameUILayout::CommandType::UpgradeSelectedTower:
        return showActions &&
               model.GetSelectedPlacedTower()->IsUpgradeable() &&
               model.GetSelectedTowerUpgradeCost(binding.upgradePathIndex) < 999999;
    case GameUILayout::CommandType::SelectBuildable:
    case GameUILayout::CommandType::StartRound:
        return true;
    }

    return false;
}

bool ExecuteButtonCommand(const GameUILayout::ButtonBinding& binding, GameModel& model) {
    if (!IsActionAvailable(binding, model)) {
        return false;
    }

    switch (binding.command) {
    case GameUILayout::CommandType::SelectBuildable:
        if (binding.buildableId[0] == '\0') {
            return false;
        }
        model.SelectBuildable(BuildableRegistry::GetInstance().FindById(binding.buildableId));
        return true;
    case GameUILayout::CommandType::StartRound:
        model.StartRound();
        return true;
    case GameUILayout::CommandType::SellSelectedTower:
        model.SellSelectedTower();
        return true;
    case GameUILayout::CommandType::UpgradeSelectedTower:
        model.UpgradeSelectedTower(binding.upgradePathIndex);
        return true;
    }

    return false;
}


void UpdateHoveredBuildable(const glm::vec2& mousePos, GameModel& model) {
    const BuildableRegistry::Entry* hoveredEntry = nullptr;

    for (const auto& binding : GameUILayout::GetTowerButtons()) {
        if (binding.hitArea.Contains(mousePos) && binding.buildableId[0] != '\0') {
            hoveredEntry = BuildableRegistry::GetInstance().FindById(binding.buildableId);
            break;
        }
    }

    model.SetHoveredBuildable(hoveredEntry);
}

bool TryExecuteClickedButton(const glm::vec2& mousePos, GameModel& model) {
    for (const auto& binding : GameUILayout::GetTowerButtons()) {
        if (binding.hitArea.Contains(mousePos)) {
            return ExecuteButtonCommand(binding, model);
        }
    }

    const auto& startButton = GameUILayout::GetStartRoundButton();
    if (startButton.hitArea.Contains(mousePos)) {
        return ExecuteButtonCommand(startButton, model);
    }

    const auto& sellButton = GameUILayout::GetSellButton();
    if (sellButton.hitArea.Contains(mousePos)) {
        return ExecuteButtonCommand(sellButton, model);
    }

    for (int pathIndex = 0; pathIndex < 2; ++pathIndex) {
        const auto& upgradeButton = GameUILayout::GetUpgradeButton(pathIndex);
        if (upgradeButton.hitArea.Contains(mousePos)) {
            return ExecuteButtonCommand(upgradeButton, model);
        }
    }

    return false;
}

void ExecuteTowerHotkeys(GameModel& model) {
    for (const auto& binding : GameUILayout::GetTowerButtons()) {
        if (binding.hotkey != Util::Keycode::UNKNOWN && Util::Input::IsKeyUp(binding.hotkey)) {
            ExecuteButtonCommand(binding, model);
        }
    }
}

} // namespace

bool GameController::ConsumeCheatSequenceInput(GameModel& model) {
    static const std::array<CheatStep, 10> kTarget = {
        CheatStep::Up, CheatStep::Up, CheatStep::Down, CheatStep::Down,
        CheatStep::Left, CheatStep::Right, CheatStep::Left, CheatStep::Right,
        CheatStep::B, CheatStep::A
    };

    std::optional<CheatStep> step;
    if (Util::Input::IsKeyUp(Util::Keycode::UP)) step = CheatStep::Up;
    else if (Util::Input::IsKeyUp(Util::Keycode::DOWN)) step = CheatStep::Down;
    else if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) step = CheatStep::Left;
    else if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) step = CheatStep::Right;
    else if (Util::Input::IsKeyUp(Util::Keycode::B)) step = CheatStep::B;
    else if (Util::Input::IsKeyUp(Util::Keycode::A)) step = CheatStep::A;

    if (!step) return false;

    m_CheatBuffer[m_CheatWriteIndex] = *step;
    m_CheatWriteIndex = (m_CheatWriteIndex + 1) % m_CheatBuffer.size();
    if (m_CheatCount < m_CheatBuffer.size()) ++m_CheatCount;
    if (m_CheatCount < kTarget.size()) return false;

    for (std::size_t i = 0; i < kTarget.size(); ++i) {
        const std::size_t idx = (m_CheatWriteIndex + i) % m_CheatBuffer.size();
        if (m_CheatBuffer[idx] != kTarget[i]) return false;
    }

    model.SetCheatMode(true);
    model.SetMessage("Cheat mode enabled. Opening cheat form...");
    return true;
}

void GameController::HandleCheatModeInput(GameModel& model) {
    (void)model;
}

void GameController::HandleInput(GameModel& model) {
    ConsumeCheatSequenceInput(model);

    ExecuteTowerHotkeys(model);

    const glm::vec2 mousePos = Util::Input::GetCursorPosition();
    UpdateHoveredBuildable(mousePos, model);
    const bool isMouseLeftUp = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
    bool consumedByHudButton = false;

    if (isMouseLeftUp) {
        consumedByHudButton = TryExecuteClickedButton(mousePos, model);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) model.StartRound();
    if (Util::Input::IsKeyUp(Util::Keycode::R)) model.Reset();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) model.CancelPlacement();
    if (Util::Input::IsKeyUp(Util::Keycode::X)) model.SellSelectedTower();

    if (isMouseLeftUp && !consumedByHudButton && model.SelectPlacedTowerAt(mousePos)) {
        consumedByHudButton = true;
    }

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);
        if (isMouseLeftUp && !consumedByHudButton) model.ConfirmPlacement();
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_RB)) model.CancelPlacement();
    }
}
