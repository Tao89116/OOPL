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
const glm::vec2 kStartButtonCenter = {535.0f, -275.0f};
const glm::vec2 kStartButtonHalfSize = {73.5f, 25.2f};
const glm::vec2 kSellButtonCenter = {535.0f, -220.0f};
const glm::vec2 kSellButtonHalfSize = {60.0f, 24.0f};
const glm::vec2 kUpgrade1ButtonCenter = {500.0f, -165.0f};
const glm::vec2 kUpgrade2ButtonCenter = {570.0f, -165.0f};
const glm::vec2 kUpgradeButtonHalfSize = {34.0f, 28.0f};

const std::array<TowerButtonBinding, 8> kTowerButtons = {{
    {{kVerticalPos,                       2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "dart_tower",      Util::Keycode::NUM_1},
    {{kVerticalPos + kVerticalInterval,   2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "track_tower",     Util::Keycode::NUM_2},
    {{kVerticalPos + 2 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "iceball_tower",   Util::Keycode::NUM_3},
    {{kVerticalPos + 3 * kVerticalInterval, 2 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "cannon_tower",    Util::Keycode::NUM_4},
    {{kVerticalPos,                       1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "boomerang_tower", Util::Keycode::NUM_5},
    {{kVerticalPos + kVerticalInterval,   1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "super_tower",     Util::Keycode::NUM_6},
    {{kVerticalPos + 2 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "spike_trap",      Util::Keycode::NUM_7},
    {{kVerticalPos + 3 * kVerticalInterval, 1 * kHorizontalInterval + kHorizontalGap}, {22.5f, 22.5f}, "glue_trap",       Util::Keycode::NUM_8}
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

    auto& registry = BuildableRegistry::GetInstance();
    const glm::vec2 mousePos = Util::Input::GetCursorPosition();
    const bool isMouseLeftUp = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
    bool consumedByHudButton = false;

    for (const auto& button : kTowerButtons) {
        if (button.buildableId[0] != '\0' && Util::Input::IsKeyUp(button.hotkey)) {
            model.SelectBuildable(registry.FindById(button.buildableId));
        }
        if (isMouseLeftUp && IsInButtonRect(mousePos, button)) {
            model.SelectBuildable(registry.FindById(button.buildableId));
            consumedByHudButton = true;
        }
    }

    if (isMouseLeftUp && IsInRect(mousePos, kStartButtonCenter, kStartButtonHalfSize)) {
        model.StartRound();
        consumedByHudButton = true;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) model.StartRound();
    //if (Util::Input::IsKeyUp(Util::Keycode::P)) model.TogglePause();
    if (Util::Input::IsKeyUp(Util::Keycode::R)) model.Reset();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) model.CancelPlacement();
    if (Util::Input::IsKeyUp(Util::Keycode::X)) model.SellSelectedTower();

    if (isMouseLeftUp && !consumedByHudButton && model.GetSelectedPlacedTower()) {
        if (model.GetSelectedPlacedTower()->IsUpgradeable() &&
            model.GetSelectedTowerUpgradeCost(0) < 999999 &&
            IsInRect(mousePos, kUpgrade1ButtonCenter, kUpgradeButtonHalfSize)) {
            model.UpgradeSelectedTower(0);
            consumedByHudButton = true;
        } else if (model.GetSelectedPlacedTower()->IsUpgradeable() &&
                   model.GetSelectedTowerUpgradeCost(1) < 999999 &&
                   IsInRect(mousePos, kUpgrade2ButtonCenter, kUpgradeButtonHalfSize)) {
            model.UpgradeSelectedTower(1);
            consumedByHudButton = true;
        } else if (IsInRect(mousePos, kSellButtonCenter, kSellButtonHalfSize)) {
            model.SellSelectedTower();
            consumedByHudButton = true;
        }
    }

    if (isMouseLeftUp && !consumedByHudButton && model.SelectPlacedTowerAt(mousePos)) {
        consumedByHudButton = true;
    }

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);
        if (isMouseLeftUp && !consumedByHudButton) model.ConfirmPlacement();
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_RB)) model.CancelPlacement();
    }
}
