#include <optional>
#include "scene/DifficultyScene.h"
#include "scene/SceneManager.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

namespace {
const std::array<DifficultyScene::CheatStep, 10> kKonami = {
    DifficultyScene::CheatStep::Up, DifficultyScene::CheatStep::Up,
    DifficultyScene::CheatStep::Down, DifficultyScene::CheatStep::Down,
    DifficultyScene::CheatStep::Left, DifficultyScene::CheatStep::Right,
    DifficultyScene::CheatStep::Left, DifficultyScene::CheatStep::Right,
    DifficultyScene::CheatStep::B, DifficultyScene::CheatStep::A,
};
}

bool DifficultyScene::ConsumeCheatSequenceInput() {
    std::optional<CheatStep> step;
    if (Util::Input::IsKeyUp(Util::Keycode::UP)) step = CheatStep::Up;
    else if (Util::Input::IsKeyUp(Util::Keycode::DOWN)) step = CheatStep::Down;
    else if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) step = CheatStep::Left;
    else if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) step = CheatStep::Right;
    else if (Util::Input::IsKeyUp(Util::Keycode::B)) step = CheatStep::B;
    else if (Util::Input::IsKeyUp(Util::Keycode::A)) step = CheatStep::A;

    if (!step) return false;
    m_CheatBuffer[m_CheatWriteIndex] = *step;
    m_CheatWriteIndex = (m_CheatWriteIndex + 1) % m_CheatBuffer.size();
    if (m_CheatCount < m_CheatBuffer.size()) ++m_CheatCount;
    if (m_CheatCount < kKonami.size()) return false;

    for (std::size_t i = 0; i < kKonami.size(); ++i) {
        std::size_t idx = (m_CheatWriteIndex + i) % m_CheatBuffer.size();
        if (m_CheatBuffer[idx] != kKonami[i]) return false;
    }
    return true;
}

void DifficultyScene::DrawCheatGui(SceneManager& sceneManager) {
    ImGui::Begin("Cheat Mode", &m_CheatMode);
    ImGui::Text("Close will return to Difficulty scene.");

    int diffIndex = static_cast<int>(sceneManager.GetDifficulty());
    const char* diffItems[] = {"Easy", "Normal", "Hard"};
    if (ImGui::Combo("Difficulty", &diffIndex, diffItems, 3)) {
        sceneManager.SetDifficulty(static_cast<DifficultyType>(diffIndex));
        m_CheatModel->SetDifficultyCheat(static_cast<DifficultyType>(diffIndex));
    }
    if (ImGui::Button("Apply Difficulty + Go Game")) {
        sceneManager.SetDifficulty(static_cast<DifficultyType>(diffIndex));
        m_CheatModel->SetDifficultyCheat(static_cast<DifficultyType>(diffIndex));
        sceneManager.SetGameSession(m_CheatModel);
        sceneManager.RequestSceneChange(SceneType::Game);
    }

    ImGui::InputInt("Gold", &m_GoldInput);
    if (ImGui::Button("Apply Gold")) m_CheatModel->SetGoldCheat(m_GoldInput);

    ImGui::InputInt("HP", &m_HPInput);
    if (ImGui::Button("Apply HP")) m_CheatModel->SetHPCheat(m_HPInput);

    ImGui::InputInt("Round", &m_RoundInput);
    if (ImGui::Button("Apply Round")) m_CheatModel->SetRoundCheat(m_RoundInput);

    const char* enemyItems[] = {"Red", "Blue", "Green", "Yellow", "Black", "White", "Lead", "Rainbow"};
    ImGui::Combo("Enemy Type", &m_EnemyTypeIndex, enemyItems, 8);
    ImGui::InputInt("Spawn Count", &m_SpawnCountInput);
    if (ImGui::Button("Spawn Enemy")) {
        m_CheatModel->SpawnEnemyCheat(static_cast<EnemyType>(m_EnemyTypeIndex), m_SpawnCountInput);
    }

    if (ImGui::Button("Force Win")) m_CheatModel->ForceWin();
    ImGui::SameLine();
    if (ImGui::Button("Force Lose")) m_CheatModel->ForceLose();

    if (ImGui::Button("Close Cheat")) {
        m_CheatMode = false;
    }
    ImGui::End();

    if (!m_CheatMode) {
        sceneManager.RequestSceneChange(SceneType::Difficulty);
    }
}

void DifficultyScene::Update(SceneManager& sceneManager) {
    if (!m_CheatMode && ConsumeCheatSequenceInput()) {
        m_CheatMode = true;
        m_CheatModel = std::make_shared<GameModel>(sceneManager.GetDifficulty());
        m_GoldInput = m_CheatModel->GetGold();
        m_HPInput = m_CheatModel->GetHP();
        m_RoundInput = m_CheatModel->GetRound();
        m_CheatModel->SetCheatMode(true);
        m_CheatModel->SetMessage("Cheat GUI opened in Difficulty scene.");
    }

    if (m_CheatMode) {
        DrawCheatGui(sceneManager);
        m_View.Render();
        return;
    }

    m_Controller.HandleInput(sceneManager);
    m_View.Render();
}
