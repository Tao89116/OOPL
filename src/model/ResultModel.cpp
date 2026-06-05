#include "model/ResultModel.h"

#include <algorithm>

namespace {
constexpr float kEnterDurationMs = 1200.0f;
}

ResultModel::ResultModel(ResultType result)
    : m_Result(result) {
}

void ResultModel::Start() {
    m_Phase = Phase::Entering;
    m_ElapsedMs = 0.0f;
}

void ResultModel::Update(float deltaTimeMs) {
    if (m_Phase == Phase::Finished) {
        return;
    }

    m_ElapsedMs += std::max(deltaTimeMs, 0.0f);

    if (m_Phase == Phase::Entering && m_ElapsedMs >= kEnterDurationMs) {
        m_Phase = Phase::Holding;
    }

    if (m_Phase == Phase::Holding) {
        m_Phase = Phase::Finished;
    }
}

float ResultModel::GetEnterProgress() const {
    return std::clamp(m_ElapsedMs / kEnterDurationMs, 0.0f, 1.0f);
}
