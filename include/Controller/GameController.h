//
// Created by polyunicorn on 2026/3/13.
//

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "model/GameModel.h"

class GameController {
public:
    void HandleInput(GameModel& model);
};

#endif