//
// Created by polyunicorn on 2026/3/26.
//

#ifndef BUILDABLE_REGISTRY_H
#define BUILDABLE_REGISTRY_H

#include "pch.hpp"
#include "model/IBuildableDefinition.h"

class BuildableRegistry {
public:
    static BuildableRegistry& GetInstance();

    void Register(const std::shared_ptr<IBuildableDefinition>& definition);
    void RegisterDefaults();

    const std::vector<std::shared_ptr<IBuildableDefinition>>& GetAll() const { return m_Definitions; }

    std::shared_ptr<IBuildableDefinition> FindById(const std::string& id) const;
    std::shared_ptr<IBuildableDefinition> GetByIndex(size_t index) const;

private:
    BuildableRegistry() = default;
    BuildableRegistry(const BuildableRegistry&) = delete;
    BuildableRegistry& operator=(const BuildableRegistry&) = delete;

private:
    std::vector<std::shared_ptr<IBuildableDefinition>> m_Definitions;
    bool m_DefaultsRegistered = false;
};

#endif