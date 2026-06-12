#include "model/GameStates.h"
#include <utility>

void ReadyState::Enter(IGameStateContext& context) {
    (void)context;
}

void ReadyState::Update(IGameStateContext& context, float deltaTimeMs) {
    context.UpdateActiveObjects(deltaTimeMs);
}

void ReadyState::Exit(IGameStateContext& context) {
    (void)context;
}

void RoundRunningState::Enter(IGameStateContext& context) {
    (void)context;
}

void RoundRunningState::Update(IGameStateContext& context, float deltaTimeMs) {
    context.UpdateRoundSpawning(deltaTimeMs);
    context.UpdateActiveObjects(deltaTimeMs);
}

void RoundRunningState::Exit(IGameStateContext& context) {
    (void)context;
}

PausedState::PausedState(std::unique_ptr<IGameState> previousState)
    : m_PreviousState(std::move(previousState)) {}

void PausedState::Enter(IGameStateContext& context) {
    (void)context;
}

void PausedState::Update(IGameStateContext& context, float deltaTimeMs) {
    (void)context;
    (void)deltaTimeMs;
}

void PausedState::Exit(IGameStateContext& context) {
    (void)context;
}

bool PausedState::IsRoundRunning() const {
    return m_PreviousState && m_PreviousState->IsRoundRunning();
}

std::unique_ptr<IGameState> PausedState::ReleasePreviousState() {
    return std::move(m_PreviousState);
}

void WinState::Enter(IGameStateContext& context) {
    (void)context;
}

void WinState::Update(IGameStateContext& context, float deltaTimeMs) {
    (void)context;
    (void)deltaTimeMs;
}

void WinState::Exit(IGameStateContext& context) {
    (void)context;
}

void LoseState::Enter(IGameStateContext& context) {
    (void)context;
}

void LoseState::Update(IGameStateContext& context, float deltaTimeMs) {
    (void)context;
    (void)deltaTimeMs;
}

void LoseState::Exit(IGameStateContext& context) {
    (void)context;
}
