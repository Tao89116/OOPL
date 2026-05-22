//
// Created by polyunicorn on 2026/3/13.
//

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "pch.hpp"
#include "SceneType.h"
#include "scene/IScene.h"
#include "model/GameModel.h"

class SceneManager {
public:
    SceneManager();

    void ChangeScene(SceneType sceneType);
    void RequestSceneChange(SceneType sceneType);
    void Update();

    void SetDifficulty(DifficultyType difficulty) { m_Difficulty = difficulty; }
    DifficultyType GetDifficulty() const { return m_Difficulty; }

    void SetResult(ResultType result) { m_Result = result; }
    ResultType GetResult() const { return m_Result; }
    std::shared_ptr<GameModel> GetGameSession() const { return m_GameSession; }
    void SetGameSession(const std::shared_ptr<GameModel>& model) { m_GameSession = model; }

private:
    void CreateScene(SceneType sceneType);

private:
    SceneType m_CurrentSceneType = SceneType::Start;
    std::unique_ptr<IScene> m_CurrentScene = nullptr;

    bool m_HasPendingSceneChange = false;
    SceneType m_PendingSceneType = SceneType::Start;

    DifficultyType m_Difficulty = DifficultyType::Easy;
    ResultType m_Result = ResultType::Lose;
    std::shared_ptr<GameModel> m_GameSession = nullptr;
};

#endif
