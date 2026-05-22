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

void DifficultyScene::Update(SceneManager& sceneManager) {
    if (ConsumeCheatSequenceInput()) {
        auto cheatModel = std::make_shared<GameModel>(sceneManager.GetDifficulty());
        cheatModel->SetCheatMode(true);
        cheatModel->SetMessage("Cheat mode opened from Difficulty. Controls: 1/2/3 diff, [/] round, ;/' gold, O/P HP, mouse wheel enemy type, -/+ count, ENTER spawn, F9/F10 lose/win, ESC close.");
        sceneManager.SetGameSession(cheatModel);
        sceneManager.RequestSceneChange(SceneType::Cheat);
        return;
    }

    m_Controller.HandleInput(sceneManager);
    m_View.Render();
}
