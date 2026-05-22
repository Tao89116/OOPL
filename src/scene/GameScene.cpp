#include "scene/GameScene.h"

#include "scene/SceneManager.h"
#include "Util/Time.hpp"

GameScene::GameScene(DifficultyType difficulty, const std::shared_ptr<GameModel>& sharedModel)
    : m_Model(sharedModel ? sharedModel : std::make_shared<GameModel>(difficulty)), m_View(difficulty) {
    m_Resources.Initialize();
    m_View.Initialize(*m_Model);
}

void GameScene::Update(SceneManager& sceneManager) {
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();

    m_Controller.HandleInput(*m_Model);
    m_Model->Update(deltaTimeMs);
    m_View.QueuePopEffects(m_Model->ConsumePoppedEnemyEvents());
    m_View.PlayPopSounds(m_Model->ConsumePoppedBloonCount());
    m_View.Render(*m_Model);

    sceneManager.SetGameSession(m_Model);

    if (m_Model->IsCheatMode()) {
        sceneManager.RequestSceneChange(SceneType::Cheat);
        return;
    }

    if (m_Model->IsWin()) {
        sceneManager.SetResult(ResultType::Win);
        sceneManager.RequestSceneChange(SceneType::Result);
    } else if (m_Model->IsLose()) {
        sceneManager.SetResult(ResultType::Lose);
        sceneManager.RequestSceneChange(SceneType::Result);
    }
}
