//
// Created by polyunicorn on 2026/3/20.
//

#ifndef MAP_FACTORY_H
#define MAP_FACTORY_H

#include "SceneType.h"
#include "model/MapData.h"

class MapFactory {
public:
    static MapData CreateByDifficulty(DifficultyType difficulty);
private:
    static const char* DifficultyToJsonKey(DifficultyType difficulty);
};

#endif