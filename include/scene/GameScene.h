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
#include "view/GameView.h"

class GameScene : public IScene {
public:
    explicit GameScene(DifficultyType difficulty);

    void Update(SceneManager& sceneManager) override;

private:
    ResourceManager& m_Resources = ResourceManager::GetInstance();
    GameModel m_Model;
    GameView m_View;
    GameController m_Controller;
};

#endif