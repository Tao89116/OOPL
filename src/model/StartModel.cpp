#include "model/StartModel.h"

#include "GameConfig.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace {
constexpr std::array<const char*, 4> kBloonSprites = {
    "bloon_0",
    "bloon_1",
    "bloon_2",
    "bloon_3"
};
}

StartModel::StartModel() {
    InitializeBloons();
    m_BananaCatPosition = {kCatStartXRight, GameConfig::StartBloonRowY[0]};
}

void StartModel::InitializeBloons() {
    m_Bloons.clear();
    std::size_t bloonCountTotal = 0;
    for (const int bloonCount : GameConfig::StartBloonsPerRow) {
        bloonCountTotal += static_cast<std::size_t>(bloonCount);
    }
    m_Bloons.reserve(bloonCountTotal);

    std::size_t spriteIndex = 0;
    for (int row = 0; row < kRowCount; ++row) {
        const int bloonCount = GameConfig::StartBloonsPerRow[row];
        const float startX = GameConfig::StartBloonStartX;
        const float endX = GameConfig::StartBloonEndX;
        const float gap = (bloonCount == 1)
            ? 0.0f
            : (endX - startX) / static_cast<float>(bloonCount - 1);

        for (int column = 0; column < bloonCount; ++column) {
            Bloon bloon;
            bloon.position = {
                startX + gap * static_cast<float>(column),
                GameConfig::StartBloonRowY[row],
            };
            bloon.spriteKey = kBloonSprites[spriteIndex % kBloonSprites.size()];
            m_Bloons.push_back(bloon);
            ++spriteIndex;
        }
    }
}

void StartModel::Update(float deltaTimeMs) {
    if (m_AnimationComplete) {
        return;
    }

    if (m_DelayRemainingMs > 0.0f) {
        m_DelayRemainingMs = std::max(0.0f, m_DelayRemainingMs - deltaTimeMs);
        return;
    }

    UpdateBananaCat(deltaTimeMs);
    PopTouchedBloons();

    if (IsCurrentRowComplete()) {
        AdvanceToNextRow();
    }
}

std::vector<StartModel::PoppedBloonEvent> StartModel::ConsumePoppedBloonEvents() {
    std::vector<PoppedBloonEvent> events = std::move(m_PoppedBloonEvents);
    m_PoppedBloonEvents.clear();
    return events;
}

void StartModel::UpdateBananaCat(float deltaTimeMs) {
    m_BananaCatPosition.x += m_RowDirection * kCatSpeedPxPerMs * deltaTimeMs;
    m_BananaCatFacingRight = m_RowDirection > 0.0f;
}

void StartModel::PopTouchedBloons() {
    const std::size_t rowStart = FirstBloonIndexInRow(m_CurrentRow);
    const std::size_t rowEnd = rowStart
        + static_cast<std::size_t>(GameConfig::StartBloonsPerRow[m_CurrentRow]);

    for (std::size_t index = rowStart; index < rowEnd; ++index) {
        auto& bloon = m_Bloons[index];
        if (bloon.popped) {
            continue;
        }

        const glm::vec2 distance = bloon.position - m_BananaCatPosition;
        const float distanceSquared = distance.x * distance.x + distance.y * distance.y;
        if (distanceSquared <= GameConfig::StartBloonCollisionRadius
                * GameConfig::StartBloonCollisionRadius) {
            bloon.popped = true;
            m_PoppedBloonEvents.push_back({bloon.position});
        }
    }
}

std::size_t StartModel::FirstBloonIndexInRow(int row) const {
    std::size_t index = 0;
    for (int currentRow = 0; currentRow < row; ++currentRow) {
        index += static_cast<std::size_t>(GameConfig::StartBloonsPerRow[currentRow]);
    }
    return index;
}

bool StartModel::IsCurrentRowComplete() const {
    const std::size_t rowStart = FirstBloonIndexInRow(m_CurrentRow);
    const std::size_t rowEnd = rowStart
        + static_cast<std::size_t>(GameConfig::StartBloonsPerRow[m_CurrentRow]);

    for (std::size_t index = rowStart; index < rowEnd; ++index) {
        if (!m_Bloons[index].popped) {
            return false;
        }
    }
    return true;
}

void StartModel::AdvanceToNextRow() {
    ++m_CurrentRow;
    if (m_CurrentRow >= kRowCount) {
        m_AnimationComplete = true;
        return;
    }

    m_RowDirection = (m_CurrentRow == 1) ? 1.0f : -1.0f;
    m_BananaCatFacingRight = m_RowDirection > 0.0f;
    m_BananaCatPosition = {
        m_RowDirection > 0.0f ? kCatStartXLeft : kCatStartXRight,
        GameConfig::StartBloonRowY[m_CurrentRow],
    };
}
