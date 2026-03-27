//
// Created by polyunicorn on 2026/3/26.
//

#include "model/BuildableRegistry.h"
#include "model/ConcreteTowers.h"

namespace {
    class DartTowerDefinition : public IBuildableDefinition {
    public:
        std::string GetId() const override { return "dart_tower"; }
        std::string GetDisplayName() const override { return "Dart Tower"; }

        int GetCost() const override { return 100; }
        float GetFootprintRadius() const override { return 26.0f; }

        bool CanPlaceOnPath() const override { return false; }
        bool ShowRangePreview() const override { return true; }
        float GetPreviewRange() const override { return 145.0f; }

        const std::string& GetPreviewSpriteKey() const override {
            static const std::string key = "tower_basic";
            return key;
        }

        std::shared_ptr<TowerBase> CreateInstance(const glm::vec2& position) const override {
            return std::make_shared<DartTower>(position);
        }
    };

    class TrackTowerDefinition : public IBuildableDefinition {
    public:
        std::string GetId() const override { return "track_tower"; }
        std::string GetDisplayName() const override { return "Track Tower"; }

        int GetCost() const override { return 140; }
        float GetFootprintRadius() const override { return 24.0f; }

        bool CanPlaceOnPath() const override { return false; }
        bool ShowRangePreview() const override { return true; }
        float GetPreviewRange() const override { return 125.0f; }

        const std::string& GetPreviewSpriteKey() const override {
            static const std::string key = "tower_track";
            return key;
        }

        std::shared_ptr<TowerBase> CreateInstance(const glm::vec2& position) const override {
            return std::make_shared<TrackTower>(position);
        }
    };

    class IceBallTowerDefinition : public IBuildableDefinition {
    public:
        std::string GetId() const override { return "IceBall_tower"; }
        std::string GetDisplayName() const override { return "IceBall Tower"; }

        int GetCost() const override { return 180; }
        float GetFootprintRadius() const override { return 30.0f; }

        bool CanPlaceOnPath() const override { return false; }
        bool ShowRangePreview() const override { return true; }
        float GetPreviewRange() const override { return 165.0f; }

        const std::string& GetPreviewSpriteKey() const override {
            static const std::string key = "tower_ice";
            return key;
        }

        std::shared_ptr<TowerBase> CreateInstance(const glm::vec2& position) const override {
            return std::make_shared<IceBallTower>(position);
        }
    };

    class SpikeTrapDefinition : public IBuildableDefinition {
    public:
        std::string GetId() const override { return "spike_trap"; }
        std::string GetDisplayName() const override { return "Spike Trap"; }

        int GetCost() const override { return 80; }
        float GetFootprintRadius() const override { return 18.0f; }

        bool CanPlaceOnPath() const override { return true; }
        bool ShowRangePreview() const override { return false; }
        float GetPreviewRange() const override { return 0.0f; }

        const std::string& GetPreviewSpriteKey() const override {
            static const std::string key = "tower_basic";
            return key;
        }

        std::shared_ptr<TowerBase> CreateInstance(const glm::vec2& position) const override {
            return std::make_shared<SpikeTrap>(position);
        }
    };
}

BuildableRegistry& BuildableRegistry::GetInstance() {
    static BuildableRegistry instance;
    return instance;
}

void BuildableRegistry::Register(const std::shared_ptr<IBuildableDefinition>& definition) {
    if (!definition) {
        return;
    }

    for (const auto& existing : m_Definitions) {
        if (existing && existing->GetId() == definition->GetId()) {
            return;
        }
    }

    m_Definitions.push_back(definition);
}

void BuildableRegistry::RegisterDefaults() {
    if (m_DefaultsRegistered) {
        return;
    }

    Register(std::make_shared<DartTowerDefinition>());
    Register(std::make_shared<TrackTowerDefinition>());
    Register(std::make_shared<IceBallTowerDefinition>());
    Register(std::make_shared<SpikeTrapDefinition>());

    m_DefaultsRegistered = true;
}

std::shared_ptr<IBuildableDefinition> BuildableRegistry::FindById(const std::string& id) const {
    for (const auto& definition : m_Definitions) {
        if (definition && definition->GetId() == id) {
            return definition;
        }
    }
    return nullptr;
}

std::shared_ptr<IBuildableDefinition> BuildableRegistry::GetByIndex(size_t index) const {
    if (index >= m_Definitions.size()) {
        return nullptr;
    }
    return m_Definitions[index];
}