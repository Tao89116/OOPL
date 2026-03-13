//
// Created by polyunicorn on 2026/3/13.
//

#ifndef SCENE_TYPE_H
#define SCENE_TYPE_H

enum class SceneType {
    Start,
    Difficulty,
    Game,
    Result
};

enum class DifficultyType {
    Easy = 0,
    Normal = 1,
    Hard = 2
};

enum class ResultType {
    Win,
    Lose
};

#endif