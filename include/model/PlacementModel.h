//
// Created by polyunicorn on 2026/3/20.
//

#ifndef PLACEMENT_MODEL_H
#define PLACEMENT_MODEL_H

#include "pch.hpp"
#include "model/TowerModel.h"

class PlacementModel {
public:
    void Begin(TowerType towerType);
    void Cancel();
    void UpdatePreviewPosition(const glm::vec2& position);
    void SetValid(bool valid);

    bool IsActive() const { return m_Active; }
    bool IsValid() const { return m_Valid; }
    TowerType GetTowerType() const { return m_TowerType; }
    const glm::vec2& GetPreviewPosition() const { return m_PreviewPosition; }

private:
    bool m_Active = false;
    bool m_Valid = false;
    TowerType m_TowerType = TowerType::Dart;
    glm::vec2 m_PreviewPosition = {0.0f, 0.0f};

    // TODO: 之後若要支援拖曳建塔、旋轉塔、或建塔 UI 選單，
    // 可在這裡加 selected card / rotation / hovered panel state。
};

#endif