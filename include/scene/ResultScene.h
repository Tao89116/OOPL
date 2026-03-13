//
// Created by polyunicorn on 2026/3/13.
//

#ifndef RESULT_SCENE_H
#define RESULT_SCENE_H

#include "scene/IScene.h"
#include "SceneType.h"
#include "view/ResultView.h"
#include "controller/ResultController.h"

class ResultScene : public IScene {
public:
    explicit ResultScene(ResultType result);

    void Update(SceneManager& sceneManager) override;

private:
    ResultView m_View;
    ResultController m_Controller;
};

#endif