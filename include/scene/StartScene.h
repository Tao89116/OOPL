#ifndef START_SCENE_H
#define START_SCENE_H

#include "controller/StartController.h"
#include "model/StartModel.h"
#include "scene/IScene.h"
#include "view/StartView.h"

class StartScene : public IScene {
public:
    StartScene() = default;
    void Update(SceneManager& sceneManager) override;

private:
    StartModel m_Model;
    StartView m_View;
    StartController m_Controller;
};

#endif
