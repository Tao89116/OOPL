//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <array>

namespace GameConfig {
    constexpr int WindowWidth = 1280;
    constexpr int WindowHeight = 720;

    constexpr float CenterX = 640.0f;
    constexpr float CenterY = 360.0f;

    constexpr int TotalRounds = 5;

    // Start scene bloon layout. Edit these values to manually move each
    // bloon row and resize the bloons so they can cover the title text in
    // the background image. Coordinates use the game-world origin at the
    // center of the window; positive Y is upward.
    constexpr std::array<float, 3> StartBloonRowY = {195.0f, 60.0f, -85.0f};
    constexpr std::array<int, 3> StartBloonsPerRow = {11, 11, 12};
    static_assert(StartBloonRowY.size() == StartBloonsPerRow.size());
    constexpr float StartBloonStartX = -470.0f;
    constexpr float StartBloonEndX = 470.0f;
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