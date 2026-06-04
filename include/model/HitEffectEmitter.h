#ifndef HIT_EFFECT_EMITTER_H
#define HIT_EFFECT_EMITTER_H

#include "model/HitEffectEvent.h"

class HitEffectEmitter {
public:
    virtual ~HitEffectEmitter() = default;
    virtual std::vector<HitEffectEvent> ConsumeHitEffectEvents() = 0;
};

#endif
