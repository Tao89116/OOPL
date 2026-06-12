#ifndef GAME_UI_LAYOUT_H
#define GAME_UI_LAYOUT_H

#include "pch.hpp"
#include "Util/Keycode.hpp"
#include <array>

namespace GameUILayout {

enum class CommandType {
    SelectBuildable,
    StartRound,
    SellSelectedTower,
    UpgradeSelectedTower,
    ReturnToDifficulty
};

struct Rect {
    glm::vec2 center;
    glm::vec2 halfSize;

    bool Contains(const glm::vec2& point) const {
        return point.x >= center.x - halfSize.x &&
               point.x <= center.x + halfSize.x &&
               point.y >= center.y - halfSize.y &&
               point.y <= center.y + halfSize.y;
    }
};

struct ButtonBinding {
    const char* spriteKey;
    CommandType command;
    Rect hitArea;
    const char* buildableId = "";
    int upgradePathIndex = -1;
    Util::Keycode hotkey = Util::Keycode::UNKNOWN;
    float renderScale = 1.0f;
};

constexpr std::size_t TowerButtonCount = 8;
constexpr float kTowerButtonScale = 0.35f;
constexpr float kActionButtonScale = 0.6f;
constexpr float kSellButtonScale = 0.65f;
constexpr float kStartButtonScale = 0.7f;
constexpr float kReturnButtonScale = 0.55f;

inline const std::array<ButtonBinding, TowerButtonCount>& GetTowerButtons() {
    static const std::array<ButtonBinding, TowerButtonCount> buttons = {{
        {"btn_tower_1", CommandType::SelectBuildable, {{462.5f, 180.0f}, {22.5f, 22.5f}},
         "dart_tower", -1, Util::Keycode::NUM_1, kTowerButtonScale},
        {"btn_tower_2", CommandType::SelectBuildable, {{507.5f, 180.0f}, {22.5f, 22.5f}},
         "track_tower", -1, Util::Keycode::NUM_2, kTowerButtonScale},
        {"btn_tower_3", CommandType::SelectBuildable, {{552.5f, 180.0f}, {22.5f, 22.5f}},
         "iceball_tower", -1, Util::Keycode::NUM_3, kTowerButtonScale},
        {"btn_tower_4", CommandType::SelectBuildable, {{597.5f, 180.0f}, {22.5f, 22.5f}},
         "cannon_tower", -1, Util::Keycode::NUM_4, kTowerButtonScale},
        {"btn_tower_5", CommandType::SelectBuildable, {{462.5f, 130.0f}, {22.5f, 22.5f}},
         "boomerang_tower", -1, Util::Keycode::NUM_5, kTowerButtonScale},
        {"btn_tower_6", CommandType::SelectBuildable, {{507.5f, 130.0f}, {22.5f, 22.5f}},
         "super_tower", -1, Util::Keycode::NUM_6, kTowerButtonScale},
        {"btn_tower_7", CommandType::SelectBuildable, {{552.5f, 130.0f}, {22.5f, 22.5f}},
         "spike_trap", -1, Util::Keycode::NUM_7, kTowerButtonScale},
        {"btn_tower_8", CommandType::SelectBuildable, {{597.5f, 130.0f}, {22.5f, 22.5f}},
         "glue_trap", -1, Util::Keycode::NUM_8, kTowerButtonScale}
    }};
    return buttons;
}

inline const ButtonBinding& GetStartRoundButton() {
    static const ButtonBinding button = {
        "btn_start",
        CommandType::StartRound,
        {{535.0f, -275.0f}, {73.5f, 25.2f}},
        "",
        -1,
        Util::Keycode::UNKNOWN,
        kStartButtonScale
    };
    return button;
}

inline const ButtonBinding& GetReturnButton() {
    static const ButtonBinding button = {
        "btn_end",
        CommandType::ReturnToDifficulty,
        {{600.0f, -325.0f}, {8.0f, 8.0f}},
        "",
        -1,
        Util::Keycode::UNKNOWN,
        kReturnButtonScale
    };
    return button;
}

inline const ButtonBinding& GetSellButton() {
    static const ButtonBinding button = {
        "btn_sell",
        CommandType::SellSelectedTower,
        {{535.0f, -220.0f}, {64.0f, 12.0f}},
        "",
        -1,
        Util::Keycode::UNKNOWN,
        kSellButtonScale
    };
    return button;
}

inline const ButtonBinding& GetUpgradeButton(int pathIndex) {
    static const std::array<ButtonBinding, 2> buttons = {{
        {"btn_upgrade_1", CommandType::UpgradeSelectedTower, {{500.0f, -120.0f}, {30.0f, 56.0f}},
         "", 0, Util::Keycode::UNKNOWN, kActionButtonScale},
        {"btn_upgrade_2", CommandType::UpgradeSelectedTower, {{570.0f, -120.0f}, {30.0f, 56.0f}},
         "", 1, Util::Keycode::UNKNOWN, kActionButtonScale}
    }};
    return buttons[static_cast<std::size_t>(pathIndex)];
}

inline glm::vec2 GetBuyItemLabelPosition() {
    return {530.0f, 225.0f};
}

inline glm::vec2 GetBuyItemUnderlinePosition() {
    return {530.0f, 215.0f};
}

inline glm::vec2 GetSelectedInfoPosition() {
    return {530.0f, 20.0f};
}

inline glm::vec2 GetSellPriceLabelPosition() {
    return {570.0f, -220.0f};
}

inline glm::vec2 GetUpgradePriceLabelPosition(int pathIndex) {
    if (pathIndex == 0) {
        return {500.0f, -165.0f};
    }

    return {570.0f, -165.0f};
}

} // namespace GameUILayout

#endif
