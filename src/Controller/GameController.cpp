//
// Created by polyunicorn on 2026/3/13.
//
#include "controller/GameController.h"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void GameController::HandleInput(GameModel& model) {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        model.SelectTower(TowerType::Dart);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        model.SelectTower(TowerType::Rapid);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        model.SelectTower(TowerType::Heavy);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::Q)) model.PlaceTowerAtSlot(0);
    if (Util::Input::IsKeyUp(Util::Keycode::W)) model.PlaceTowerAtSlot(1);
    if (Util::Input::IsKeyUp(Util::Keycode::E)) model.PlaceTowerAtSlot(2);
    if (Util::Input::IsKeyUp(Util::Keycode::A)) model.PlaceTowerAtSlot(3);
    if (Util::Input::IsKeyUp(Util::Keycode::S)) model.PlaceTowerAtSlot(4);
    if (Util::Input::IsKeyUp(Util::Keycode::D)) model.PlaceTowerAtSlot(5);

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        model.StartRound();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::P)) {
        model.TogglePause();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::R)) {
        model.Reset();
    }
    // cheating for test
    if (Util::Input::IsKeyUp(Util::Keycode::F9)) {
        model.ForceLose();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::F10)) {
        model.ForceWin();
    }
    // TODO:
    // 後續若要改成滑鼠互動，這個 Controller 可以補：
    // - 點擊 tower button
    // - slot去除 點擊建塔
    // - 點擊已建好的塔查看資訊
}