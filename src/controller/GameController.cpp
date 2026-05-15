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
const glm::vec2 kStartButtonCenter = {460.0f, -115.0f};
const glm::vec2 kStartButtonHalfSize = {52.5f, 18.0f};

const std::array<TowerButtonBinding, 8> kTowerButtons = {{
    {{kVerticalPos,                       2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "dart_tower",    Util::Keycode::NUM_1},
    {{kVerticalPos + kVerticalInterval,   2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "track_tower",   Util::Keycode::NUM_2},
    {{kVerticalPos + 2 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "iceball_tower",  Util::Keycode::NUM_3},
    {{kVerticalPos + 3 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "cannon_tower",    Util::Keycode::NUM_4},
    {{kVerticalPos,                       1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "boomerang_tower",   Util::Keycode::NUM_5},
    {{kVerticalPos + kVerticalInterval,   1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "super_tower", Util::Keycode::NUM_6},
    {{kVerticalPos + 2 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "spike_trap",    Util::Keycode::NUM_7},
    {{kVerticalPos + 3 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "glue_trap",              Util::Keycode::NUM_8}
}};

bool IsInRect(const glm::vec2& point, const glm::vec2& center, const glm::vec2& halfSize) {
    return point.x >= center.x - halfSize.x &&
           point.x <= center.x + halfSize.x &&
           point.y >= center.y - halfSize.y &&
           point.y <= center.y + halfSize.y;
}

bool IsInButtonRect(const glm::vec2& point, const TowerButtonBinding& button) {
    return IsInRect(point, button.center, button.halfSize);
}

} // namespace

void GameController::HandleInput(GameModel& model) {
    auto& registry = BuildableRegistry::GetInstance();
    const glm::vec2 mousePos = Util::Input::GetCursorPosition();
    const bool isMouseLeftUp = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
    bool consumedByHudButton = false;

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
            consumedByHudButton = true;
        }
    }

    if (isMouseLeftUp && IsInRect(mousePos, kStartButtonCenter, kStartButtonHalfSize)) {
        model.StartRound();
        consumedByHudButton = true;
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
    if (Util::Input::IsKeyUp(Util::Keycode::X)) {
        model.SellSelectedTower();
    }

    if (isMouseLeftUp && !consumedByHudButton) {
        if (model.SelectPlacedTowerAt(mousePos)) {
            consumedByHudButton = true;
        }
    }

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);

        if (isMouseLeftUp && !consumedByHudButton) {
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
