#ifndef CHEAT_SCENE_H
#define CHEAT_SCENE_H

#include "scene/IScene.h"
#include "model/GameModel.h"

class CheatScene : public IScene {
public:
    explicit CheatScene(const std::shared_ptr<GameModel>& model);
    void Update(SceneManager& sceneManager) override;

private:
    std::shared_ptr<GameModel> m_Model;
};

#endif
