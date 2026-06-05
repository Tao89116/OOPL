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
    constexpr std::array<float, 3> StartBloonRowY = {185.0f, 35.0f, -110.0f};
    constexpr std::array<int, 3> StartBloonsPerRow = {6, 5, 7};
    constexpr std::size_t StartBloonMaxColumns = 7;
    constexpr std::array<std::array<float, StartBloonMaxColumns>, 3> StartBloonRowX = {{
        {{-324.0f, -195.2f, -66.4f, 62.4f, 184.2f, 320.0f}},
        {{-259.6f, -130.8f, 0.0f, 130.8f, 252.6f}},
        {{-391.5f, -264.5f, -142.6f, 0.0f, 123.6f, 255.5f, 383.5f}},
    }};
    // static_assert(StartBloonRowY.size() == StartBloonsPerRow.size());
    // static_assert(StartBloonRowX.size() == StartBloonsPerRow.size());
    // static_assert(StartBloonsPerRow[0] <= static_cast<int>(StartBloonRowX[0].size()));
    // static_assert(StartBloonsPerRow[1] <= static_cast<int>(StartBloonRowX[1].size()));
    // static_assert(StartBloonsPerRow[2] <= static_cast<int>(StartBloonRowX[2].size()));
    constexpr float StartBloonWidth = 100.0f;
    constexpr float StartBloonHeight = 130.0f;
    constexpr float StartBloonCollisionRadius = 84.0f;

    // constexpr float TopBarY = 36.0f;
    // constexpr float BottomBarY = 690.0f;

    constexpr float RightPanelX = 1130.0f;
    constexpr float RightPanelButtonStartY = 170.0f;
    constexpr float RightPanelButtonGap = 95.0f;
}

#endif