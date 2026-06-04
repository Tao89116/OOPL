//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <array>
#include <cstddef>

namespace GameConfig {
    constexpr int WindowWidth = 1280;
    constexpr int WindowHeight = 720;

    constexpr float CenterX = 640.0f;
    constexpr float CenterY = 360.0f;

    constexpr int TotalRounds = 50;

    // Start scene bloon layout. Edit these values to manually move each
    // bloon row and resize the bloons so they can cover the title text in
    // the background image. Coordinates use the game-world origin at the
    // center of the window; positive Y is upward.
    constexpr std::array<float, 3> StartBloonRowY = {195.0f, 35.0f, -110.0f};
    constexpr std::array<int, 3> StartBloonsPerRow = {6, 5, 7};
    constexpr std::size_t StartBloonMaxColumns = 7;
    constexpr std::array<std::array<float, StartBloonMaxColumns>, 3> StartBloonRowX = {{
        {{-470.0f, -282.0f, -94.0f, 94.0f, 282.0f, 470.0f, 0.0f}},
        {{-470.0f, -235.0f, 0.0f, 235.0f, 470.0f, 0.0f, 0.0f}},
        {{-470.0f, -313.333f, -156.667f, 0.0f, 156.667f, 313.333f, 470.0f}},
    }};
    static_assert(StartBloonRowY.size() == StartBloonsPerRow.size());
    static_assert(StartBloonRowX.size() == StartBloonsPerRow.size());
    static_assert(StartBloonsPerRow[0] <= static_cast<int>(StartBloonRowX[0].size()));
    static_assert(StartBloonsPerRow[1] <= static_cast<int>(StartBloonRowX[1].size()));
    static_assert(StartBloonsPerRow[2] <= static_cast<int>(StartBloonRowX[2].size()));
    constexpr float StartBloonWidth = 96.0f;
    constexpr float StartBloonHeight = 126.0f;
    constexpr float StartBloonCollisionRadius = 84.0f;

    constexpr float TopBarY = 36.0f;
    constexpr float BottomBarY = 690.0f;

    constexpr float RightPanelX = 1130.0f;
    constexpr float RightPanelButtonStartY = 170.0f;
    constexpr float RightPanelButtonGap = 95.0f;
}

#endif