#ifndef HIT_EFFECT_EVENT_H
#define HIT_EFFECT_EVENT_H

#include "pch.hpp"

struct HitEffectEvent {
    std::string imageKey;
    std::string soundKey;
    glm::vec2 position = {0.0f, 0.0f};
    float scale = 1.0f;
    float durationMs = 200.0f;
};

#endif
