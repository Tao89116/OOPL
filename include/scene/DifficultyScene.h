//
// Created by polyunicorn on 2026/3/13.
//

#ifndef DIFFICULTY_SCENE_H
#define DIFFICULTY_SCENE_H

#include "scene/IScene.h"
#include "view/DifficultyView.h"
#include "controller/DifficultyController.h"

class DifficultyScene : public IScene {
public:
    DifficultyScene() = default;
    void Update(SceneManager& sceneManager) override;

private:
    DifficultyView m_View;
    DifficultyController m_Controller;
};

#endif