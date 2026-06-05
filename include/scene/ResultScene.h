//
// Created by polyunicorn on 2026/3/13.
//

#ifndef RESULT_SCENE_H
#define RESULT_SCENE_H

#include "scene/IScene.h"
#include "SceneType.h"
#include "view/ResultView.h"
#include "controller/ResultController.h"
#include "model/GameModel.h"
#include "model/ResultModel.h"
#include "view/GameView.h"

class ResultScene : public IScene {
public:
    explicit ResultScene(
        ResultType result,
        DifficultyType difficulty,
        const std::shared_ptr<GameModel>& finishedGame = nullptr
    );

    void Update(SceneManager& sceneManager) override;

private:
    ResultModel m_Model;
    ResultView m_View;
    std::shared_ptr<GameModel> m_FinishedGame = nullptr;
    std::unique_ptr<GameView> m_GameBackdrop = nullptr;
    ResultController m_Controller;
    bool m_Started = false;
};

#endif
