#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include <memory>

class IGameStateContext {
public:
    virtual ~IGameStateContext() = default;

    virtual void UpdateRoundSpawning(float deltaTimeMs) = 0;
    virtual void UpdateActiveObjects(float deltaTimeMs) = 0;
};

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void Enter(IGameStateContext& context) = 0;
    virtual void Update(IGameStateContext& context, float deltaTimeMs) = 0;
    virtual void Exit(IGameStateContext& context) = 0;

    virtual const char* GetName() const = 0;

    virtual bool IsRoundRunning() const { return false; }
    virtual bool IsPaused() const { return false; }
    virtual bool IsWin() const { return false; }
    virtual bool IsLose() const { return false; }
    virtual std::unique_ptr<IGameState> ReleasePreviousState() { return nullptr; }
};

#endif
