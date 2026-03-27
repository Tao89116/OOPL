//
// Created by polyunicorn on 2026/3/26.
//

#ifndef I_BUILDABLE_DEFINITION_H
#define I_BUILDABLE_DEFINITION_H

#include "pch.hpp"

class TowerBase;

class IBuildableDefinition {
public:
    virtual ~IBuildableDefinition() = default;

    virtual std::string GetId() const = 0;
    virtual std::string GetDisplayName() const = 0;

    virtual int GetCost() const = 0;
    virtual float GetFootprintRadius() const = 0;

    virtual bool CanPlaceOnPath() const = 0;
    virtual bool ShowRangePreview() const = 0;
    virtual float GetPreviewRange() const = 0;

    virtual const std::string& GetPreviewSpriteKey() const = 0;

    virtual std::shared_ptr<TowerBase> CreateInstance(const glm::vec2& position) const = 0;

    // TODO:
    // 之後若要做升級系統，可在 Definition 層補：
    // - 升級路徑說明
    // - 升級費用
    // - 升級名稱 / 敘述
};

#endif