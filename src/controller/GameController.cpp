//
// Created by polyunicorn on 2026/3/13.
//

#include "controller/GameController.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <array>

namespace {

struct TowerButtonBinding {
    glm::vec2 center;
    glm::vec2 halfSize;
    const char* buildableId;
    Util::Keycode hotkey;
};

constexpr float kVerticalPos = 462.5f;
constexpr float kVerticalInterval = 45.0f;
constexpr float kHorizontalGap = 80.0f;
constexpr float kHorizontalInterval = 50.0f;

const std::array<TowerButtonBinding, 8> kTowerButtons = {{
    {{kVerticalPos,                       2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "dart_tower",    Util::Keycode::NUM_1},
    {{kVerticalPos + kVerticalInterval,   2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "track_tower",   Util::Keycode::NUM_2},
    {{kVerticalPos + 2 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "iceball_tower",  Util::Keycode::NUM_3},
    {{kVerticalPos + 3 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "cannon_tower",    Util::Keycode::NUM_4},
    {{kVerticalPos,                       1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "",   Util::Keycode::NUM_5},//boomerang
    {{kVerticalPos + kVerticalInterval,   1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "super_tower", Util::Keycode::NUM_6},
    {{kVerticalPos + 2 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "spike_trap",    Util::Keycode::NUM_7},
    {{kVerticalPos + 3 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "glue_tower",              Util::Keycode::NUM_8}
}};

bool IsInButtonRect(const glm::vec2& point, const TowerButtonBinding& button) {
    return point.x >= button.center.x - button.halfSize.x &&
           point.x <= button.center.x + button.halfSize.x &&
           point.y >= button.center.y - button.halfSize.y &&
           point.y <= button.center.y + button.halfSize.y;
}

} // namespace

void GameController::HandleInput(GameModel& model) {
    auto& registry = BuildableRegistry::GetInstance();
    const glm::vec2 mousePos = Util::Input::GetCursorPosition();
    const bool isMouseLeftUp = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
    bool consumedByTowerButton = false;

    for (const auto& button : kTowerButtons) {
        if (button.buildableId[0] != '\0' && Util::Input::IsKeyUp(button.hotkey)) {
            model.SelectBuildable(registry.FindById(button.buildableId));
        }

        if (isMouseLeftUp && IsInButtonRect(mousePos, button)) {
            if (button.buildableId[0] != '\0') {
                model.SelectBuildable(registry.FindById(button.buildableId));
            } else {
                model.SetMessage("This tower button is not available yet.");
            }
            consumedByTowerButton = true;
        }
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

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);

        if (isMouseLeftUp && !consumedByTowerButton) {
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
