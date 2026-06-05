#include "scene/GameScene.h"

#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

GameScene::GameScene(DifficultyType difficulty, const std::shared_ptr<GameModel>& sharedModel)
    : m_Model(sharedModel ? sharedModel : std::make_shared<GameModel>(difficulty)), m_View(difficulty) {
    m_Resources.Initialize();
    m_View.Initialize(*m_Model);
    m_GoldInput = m_Model->GetGold();
    m_HPInput = m_Model->GetHP();
    m_RoundInput = m_Model->GetRound();
    m_DifficultyIndex = static_cast<int>(difficulty);
}

void GameScene::DrawCheatGui(SceneManager& sceneManager) {
    if (!m_Model || !m_Model->IsCheatMode()) {
        return;
    }

    ImGui::Begin("Cheat Mode");

    const char* diffItems[] = {"Easy", "Normal", "Hard"};
    ImGui::Combo("Difficulty", &m_DifficultyIndex, diffItems, 3);
    if (ImGui::Button("Apply Difficulty")) {
        sceneManager.SetDifficulty(static_cast<DifficultyType>(m_DifficultyIndex));
        m_Model->SetDifficultyCheat(static_cast<DifficultyType>(m_DifficultyIndex));
    }

    ImGui::InputInt("Gold", &m_GoldInput);
    if (ImGui::Button("Apply Gold")) m_Model->SetGoldCheat(m_GoldInput);

    ImGui::InputInt("HP", &m_HPInput);
    if (ImGui::Button("Apply HP")) m_Model->SetHPCheat(m_HPInput);

    ImGui::InputInt("Round", &m_RoundInput);
    if (ImGui::Button("Apply Round")) m_Model->SetRoundCheat(m_RoundInput);

    const char* enemyItems[] = {"Red", "Blue", "Green", "Yellow", "Black", "White", "Lead", "Rainbow"};
    ImGui::Combo("Enemy Type", &m_EnemyTypeIndex, enemyItems, 8);
    ImGui::InputInt("Spawn Count", &m_SpawnCountInput);
    if (ImGui::Button("Spawn")) {
        m_Model->SpawnEnemyCheat(static_cast<EnemyType>(m_EnemyTypeIndex), m_SpawnCountInput);
    }

    if (ImGui::Button("Pause")) {
        m_Model->TogglePause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        m_Model->Reset();
    }

    if (ImGui::Button("Force Win")) m_Model->ForceWin();
    ImGui::SameLine();
    if (ImGui::Button("Force Lose")) m_Model->ForceLose();

    if (ImGui::Button("Close Cheat GUI")) {
        m_Model->SetCheatMode(false);
        m_Model->SetMessage("Cheat GUI closed.");
    }

    ImGui::End();
}

void GameScene::Update(SceneManager& sceneManager) {
    const float deltaTimeMs = Util::Time::GetDeltaTimeMs();

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
        m_View.IsReturnButtonHit(Util::Input::GetCursorPosition())) {
        sceneManager.SetGameSession(nullptr);
        sceneManager.RequestSceneChange(SceneType::Difficulty);
        return;
    }

    m_Controller.HandleInput(*m_Model);
    m_Model->Update(deltaTimeMs);
    m_View.QueuePopEffects(m_Model->ConsumePoppedEnemyEvents());
    m_View.QueueHitEffects(m_Model->ConsumeHitEffectEvents());
    m_View.PlayPopSounds(m_Model->ConsumePoppedBloonCount());
    m_View.Render(*m_Model);
    DrawCheatGui(sceneManager);

    sceneManager.SetGameSession(m_Model);

    if (m_Model->IsWin()) {
        sceneManager.SetResult(ResultType::Win);
        sceneManager.RequestSceneChange(SceneType::Result);
    } else if (m_Model->IsLose()) {
        sceneManager.SetResult(ResultType::Lose);
        sceneManager.RequestSceneChange(SceneType::Result);
    }
}
