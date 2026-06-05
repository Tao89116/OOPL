#include "model/ResultModel.h"

#include <algorithm>

namespace {
constexpr float kEnterDurationMs = 1200.0f;
constexpr float kReturnDelayAfterSoundMs = 3000.0f;
}

ResultModel::ResultModel(ResultType result)
    : m_Result(result) {
}

void ResultModel::Start(float soundDurationMs) {
    m_Phase = Phase::Entering;
    m_ElapsedMs = 0.0f;
    m_SoundDurationMs = std::max(soundDurationMs, kEnterDurationMs);
}

void ResultModel::Update(float deltaTimeMs) {
    if (m_Phase == Phase::Finished) {
        return;
    }

    m_ElapsedMs += std::max(deltaTimeMs, 0.0f);

    if (m_Phase == Phase::Entering && m_ElapsedMs >= kEnterDurationMs) {
        m_Phase = Phase::Holding;
    }

    if (m_ElapsedMs >= m_SoundDurationMs + kReturnDelayAfterSoundMs) {
        m_Phase = Phase::Finished;
    }
}

float ResultModel::GetEnterProgress() const {
    return std::clamp(m_ElapsedMs / kEnterDurationMs, 0.0f, 1.0f);
}
