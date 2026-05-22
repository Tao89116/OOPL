#include "scene/CheatScene.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

CheatScene::CheatScene(const std::shared_ptr<GameModel>& model)
    : m_Model(model) {
    if (m_Model) {
        m_Model->SetCheatMode(true);
        m_Model->SetMessage("Cheat form opened. Scroll: enemy type, +/-: count, 1/2/3 diff, [/] round, ;/' gold, ENTER spawn, F9 lose, F10 win, ESC close");
    }
}

void CheatScene::Update(SceneManager& sceneManager) {
    if (!m_Model) {
        sceneManager.RequestSceneChange(SceneType::Game);
        return;
    }

    static int enemyType = 0;
    static int spawnCount = 1;

    if (Util::Input::IfScroll()) {
        const float y = Util::Input::GetScrollDistance().y;
        if (y > 0.0f) enemyType = (enemyType + 1) % 8;
        if (y < 0.0f) enemyType = (enemyType + 7) % 8;
        m_Model->SetMessage("Cheat form: enemy type set to index " + std::to_string(enemyType));
    }

    if (Util::Input::IsKeyUp(Util::Keycode::EQUALS)) spawnCount = std::min(99, spawnCount + 1);
    if (Util::Input::IsKeyUp(Util::Keycode::MINUS)) spawnCount = std::max(1, spawnCount - 1);

    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) m_Model->SetDifficultyCheat(DifficultyType::Easy);
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) m_Model->SetDifficultyCheat(DifficultyType::Normal);
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) m_Model->SetDifficultyCheat(DifficultyType::Hard);

    if (Util::Input::IsKeyUp(Util::Keycode::LEFTBRACKET)) m_Model->SetRoundCheat(m_Model->GetRound() - 1);
    if (Util::Input::IsKeyUp(Util::Keycode::RIGHTBRACKET)) m_Model->SetRoundCheat(m_Model->GetRound() + 1);

    if (Util::Input::IsKeyUp(Util::Keycode::SEMICOLON)) m_Model->SetGoldCheat(std::max(0, m_Model->GetGold() - 100));
    if (Util::Input::IsKeyUp(Util::Keycode::APOSTROPHE)) m_Model->SetGoldCheat(m_Model->GetGold() + 100);

    if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) m_Model->SpawnEnemyCheat(static_cast<EnemyType>(enemyType), spawnCount);
    if (Util::Input::IsKeyUp(Util::Keycode::F9)) m_Model->ForceLose();
    if (Util::Input::IsKeyUp(Util::Keycode::F10)) m_Model->ForceWin();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_Model->SetCheatMode(false);
        m_Model->SetMessage("Cheat form closed.");
        sceneManager.RequestSceneChange(SceneType::Game);
    }
}
