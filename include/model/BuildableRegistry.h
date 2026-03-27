#ifndef BUILDABLE_REGISTRY_H
#define BUILDABLE_REGISTRY_H

#include "pch.hpp"
#include <functional>
#include "model/IBuildable.h"

class BuildableRegistry {
public:
    using FactoryFn = std::function<std::shared_ptr<IBuildable>(const glm::vec2&)>;

    struct Entry {
        std::string id;
        FactoryFn factory;
    };

public:
    static BuildableRegistry& GetInstance();

    void Register(const std::string& id, FactoryFn factory);
    void RegisterDefaults();

    const std::vector<Entry>& GetAll() const;
    const Entry* FindById(const std::string& id) const;
    const Entry* GetByIndex(size_t index) const;

private:
    BuildableRegistry() = default;
    BuildableRegistry(const BuildableRegistry&) = delete;
    BuildableRegistry& operator=(const BuildableRegistry&) = delete;

private:
    std::vector<Entry> m_Entries;
    bool m_DefaultsRegistered = false;
};

#endif