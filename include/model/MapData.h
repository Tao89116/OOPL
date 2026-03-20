//
// Created by polyunicorn on 2026/3/20.
//

#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "pch.hpp"
//因為多條路徑 改成預設多路徑
struct MapData {
    std::string backgroundKey;
    std::vector<std::vector<glm::vec2>> paths;
    float pathWidth = 55.0f;
};

#endif