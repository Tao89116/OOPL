//
// Created by polyunicorn on 2026/3/13.
//
#include "model/MapModel.h"

MapModel::MapModel() {
    m_PathPoints = {
        {70.0f, 360.0f},
        {250.0f, 360.0f},
        {250.0f, 180.0f},
        {520.0f, 180.0f},
        {520.0f, 520.0f},
        {840.0f, 520.0f},
        {840.0f, 290.0f},
        {1130.0f, 290.0f}
    };

    m_TowerSlots = {
        {160.0f, 220.0f},
        {360.0f, 300.0f},
        {410.0f, 590.0f},
        {650.0f, 340.0f},
        {720.0f, 120.0f},
        {980.0f, 460.0f}
    };

    // TODO:
    // 之後不同難度若要使用不同地圖路徑，可改成：
    // 1. MapModel 接受 difficulty 或 map id
    // 2. 每個地圖各自有 tower slots / path points
    // 3. 再把這些資料抽成 json / txt / csv
}