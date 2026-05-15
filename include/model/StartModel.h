#ifndef START_MODEL_H
#define START_MODEL_H

#include "GameConfig.h"
#include "pch.hpp"
#include <cstddef>
#include <string>
#include <vector>

class StartModel {
public:
    struct PoppedBloonEvent {
        glm::vec2 position = {0.0f, 0.0f};
    };

    struct Bloon {
        glm::vec2 position;
        std::string spriteKey;
        bool popped = false;
    };

    StartModel();

    void Update(float deltaTimeMs);
    std::vector<PoppedBloonEvent> ConsumePoppedBloonEvents();

    const std::vector<Bloon>& GetBloons() const { return m_Bloons; }
    const glm::vec2& GetBananaCatPosition() const { return m_BananaCatPosition; }
    bool IsBananaCatFacingRight() const { return m_BananaCatFacingRight; }
    bool IsAnimationComplete() const { return m_AnimationComplete; }

private:
    void InitializeBloons();
    void UpdateBananaCat(float deltaTimeMs);
    void PopTouchedBloons();
    std::size_t FirstBloonIndexInRow(int row) const;
    bool IsCurrentRowComplete() const;
    void AdvanceToNextRow();

private:
    std::vector<Bloon> m_Bloons;
    std::vector<PoppedBloonEvent> m_PoppedBloonEvents;
    glm::vec2 m_BananaCatPosition = {760.0f, 0.0f};
    bool m_BananaCatFacingRight = false;
    bool m_AnimationComplete = false;

    float m_DelayRemainingMs = 2000.0f;
    int m_CurrentRow = 0;
    float m_RowDirection = -1.0f;

    static constexpr int kRowCount = static_cast<int>(GameConfig::StartBloonRowY.size());
    static constexpr float kCatStartXRight = 760.0f;
    static constexpr float kCatStartXLeft = -760.0f;
    static constexpr float kCatSpeedPxPerMs = 0.42f;
};

#endif
