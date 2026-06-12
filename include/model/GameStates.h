#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "model/IGameState.h"
#include <memory>

class ReadyState final : public IGameState {
public:
    void Enter(IGameStateContext& context) override;
    void Update(IGameStateContext& context, float deltaTimeMs) override;
    void Exit(IGameStateContext& context) override;

    const char* GetName() const override { return "Ready"; }
};

class RoundRunningState final : public IGameState {
public:
    void Enter(IGameStateContext& context) override;
    void Update(IGameStateContext& context, float deltaTimeMs) override;
    void Exit(IGameStateContext& context) override;

    const char* GetName() const override { return "RoundRunning"; }
    bool IsRoundRunning() const override { return true; }
};

class PausedState final : public IGameState {
public:
    explicit PausedState(std::unique_ptr<IGameState> previousState);

    void Enter(IGameStateContext& context) override;
    void Update(IGameStateContext& context, float deltaTimeMs) override;
    void Exit(IGameStateContext& context) override;

    const char* GetName() const override { return "Paused"; }
    bool IsPaused() const override { return true; }
    bool IsRoundRunning() const override;

    std::unique_ptr<IGameState> ReleasePreviousState() override;

private:
    std::unique_ptr<IGameState> m_PreviousState;
};

class WinState final : public IGameState {
public:
    void Enter(IGameStateContext& context) override;
    void Update(IGameStateContext& context, float deltaTimeMs) override;
    void Exit(IGameStateContext& context) override;

    const char* GetName() const override { return "Win"; }
    bool IsWin() const override { return true; }
};

class LoseState final : public IGameState {
public:
    void Enter(IGameStateContext& context) override;
    void Update(IGameStateContext& context, float deltaTimeMs) override;
    void Exit(IGameStateContext& context) override;

    const char* GetName() const override { return "Lose"; }
    bool IsLose() const override { return true; }
};

#endif
