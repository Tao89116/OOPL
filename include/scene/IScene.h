//
// Created by polyunicorn on 2026/3/13.
//

#ifndef I_SCENE_H
#define I_SCENE_H

class SceneManager;

class IScene {
public:
    virtual ~IScene() = default;
    virtual void Update(SceneManager& sceneManager) = 0;
};

#endif