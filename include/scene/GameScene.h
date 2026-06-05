//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "scene/IScene.h"
#include "SceneType.h"
#include "ResourceManager.h"
#include "controller/GameController.h"
#include "model/GameModel.h"
#include "model/ResultModel.h"
#include "view/GameView.h"

class GameScene : public IScene {
public:
    explicit GameScene(DifficultyType difficulty, const std::shared_ptr<GameModel>& sharedModel = nullptr);

    void Update(SceneManager& sceneManager) override;

private:
    bool HandleReturnToDifficulty(SceneManager& sceneManager);
    void UpdateGameFrame(float deltaTimeMs);
    void UpdateResultState(SceneManager& sceneManager, float deltaTimeMs);
    void StartResultState(ResultType result);
    void DrawCheatGui(SceneManager& sceneManager);

    ResourceManager& m_Resources = ResourceManager::GetInstance();
    std::shared_ptr<GameModel> m_Model;
    GameView m_View;
    GameController m_Controller;
    std::unique_ptr<ResultModel> m_ResultModel = nullptr;

    int m_GoldInput = 0;
    int m_HPInput = 0;
    int m_RoundInput = 1;
    int m_DifficultyIndex = 0;
    int m_EnemyTypeIndex = 0;
    int m_SpawnCountInput = 1;
};

#endif
