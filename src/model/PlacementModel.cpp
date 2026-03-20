//
// Created by polyunicorn on 2026/3/20.
//

#include "model/PlacementModel.h"

void PlacementModel::Begin(TowerType towerType) {
    m_Active = true;
    m_TowerType = towerType;
    m_PreviewPosition = {0.0f, 0.0f};
    m_Valid = false;
}

void PlacementModel::Cancel() {
    m_Active = false;
    m_Valid = false;
}

void PlacementModel::UpdatePreviewPosition(const glm::vec2& position) {
    m_PreviewPosition = position;
}

void PlacementModel::SetValid(bool valid) {
    m_Valid = valid;
}