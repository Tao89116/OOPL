//
// Created by polyunicorn on 2026/3/13.
//

#ifndef START_SCENE_H
#define START_SCENE_H

#include "scene/IScene.h"
#include "view/StartView.h"
#include "controller/StartController.h"

class StartScene : public IScene {
public:
    StartScene() = default;
    void Update(SceneManager& sceneManager) override;

private:
    StartView m_View;
    StartController m_Controller;
};

#endif