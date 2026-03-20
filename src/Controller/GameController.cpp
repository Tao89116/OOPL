//
// Created by polyunicorn on 2026/3/13.
//

#include "controller/GameController.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
//test
#include <iostream>

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

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        model.StartRound();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::P)) {
        model.TogglePause();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::R)) {
        model.Reset();
    }
    //test start here
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        model.CancelPlacement();
    }
    // 不管有沒有在 placement mode，都先更新目前滑鼠位置
    const glm::vec2 mousePos = Util::Input::GetCursorPosition();

    if (model.GetPlacement().IsActive()) {
        model.UpdatePlacementPreview(mousePos);
    }

    // 左鍵測試：顯示目前點擊座標
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        const glm::vec2 mousePos = Util::Input::GetCursorPosition();

        std::cout << "Mouse Click: ("
                  << mousePos.x << ", "
                  << mousePos.y << ")"
                  << std::endl;
    }//TODO 接近1280X720 路寬約55
    if (model.GetPlacement().IsActive()) {
        const glm::vec2 mousePos = Util::Input::GetCursorPosition();
        model.UpdatePlacementPreview(mousePos);

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            model.ConfirmPlacement();
        }

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_RB)) {
            model.CancelPlacement();
        }
    }

    //cheat
    if (Util::Input::IsKeyUp(Util::Keycode::F9)) {
        model.ForceLose();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::F10)) {
        model.ForceWin();
    }
}