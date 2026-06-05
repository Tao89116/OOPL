#ifndef RESULT_MODEL_H
#define RESULT_MODEL_H

#include "SceneType.h"

class ResultModel {
public:
    enum class Phase {
        Entering,
        Holding,
        Finished
    };

    explicit ResultModel(ResultType result);

    void Start(float soundDurationMs);
    void Update(float deltaTimeMs);

    ResultType GetResult() const { return m_Result; }
    Phase GetPhase() const { return m_Phase; }
    float GetElapsedMs() const { return m_ElapsedMs; }
    float GetEnterProgress() const;
    bool IsFinished() const { return m_Phase == Phase::Finished; }

private:
    ResultType m_Result;
    Phase m_Phase = Phase::Entering;
    float m_ElapsedMs = 0.0f;
    float m_SoundDurationMs = 0.0f;
};

#endif
