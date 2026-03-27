//
// Created by polyunicorn on 2026/3/13.
//

#include "controller/GameController.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "model/BuildableRegistry.h"

void GameController::HandleInput(GameModel& model) {
    auto& registry = BuildableRegistry::GetInstance();

    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        model.SelectBuildable(registry.GetByIndex(0));
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        model.SelectBuildable(registry.GetByIndex(1));
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        model.SelectBuildable(registry.GetByIndex(2));
    }

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        model.StartRound();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::P)) {
        model.TogglePause();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::R)) {
        model.Reset();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        model.CancelPlacement();
    }

    const glm::vec2 mousePos = Util::Input::GetCursorPosition();

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            model.ConfirmPlacement();
        }

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_RB)) {
            model.CancelPlacement();
        }
    }

    //cheat
    if (Util::Input::IsKeyUp(Util::Keycode::F9)) {
        model.ForceLose();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::F10)) {
        model.ForceWin();
    }
}