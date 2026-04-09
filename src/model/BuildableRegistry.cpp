#include "model/BuildableRegistry.h"
#include "model/ConcreteTowers.h"

BuildableRegistry& BuildableRegistry::GetInstance() {
    static BuildableRegistry instance;
    return instance;
}

void BuildableRegistry::Register(const std::string& id, FactoryFn factory) {
    for (const auto& entry : m_Entries) {
        if (entry.id == id) {
            return;
        }
    }

    m_Entries.push_back({id, factory});
}

void BuildableRegistry::RegisterDefaults() {
    if (m_DefaultsRegistered) {
        return;
    }

    Register("dart_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<DartTower>(position);
    });

    Register("track_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<TrackTower>(position);
    });

    Register("cannon_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<CannonTower>(position);
    });

    Register("glue_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<GlueTower>(position);
    });

    Register("super_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<SuperTower>(position);
    });

    Register("iceball_tower", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<IceBallTower>(position);
    });

    Register("spike_trap", [](const glm::vec2& position) -> std::shared_ptr<IBuildable> {
        return std::make_shared<SpikeTrap>(position);
    });

    // TODO:
    // 新塔新增時，只要在這裡註冊 factory 即可

    m_DefaultsRegistered = true;
}

const std::vector<BuildableRegistry::Entry>& BuildableRegistry::GetAll() const {
    return m_Entries;
}

const BuildableRegistry::Entry* BuildableRegistry::FindById(const std::string& id) const {
    for (const auto& entry : m_Entries) {
        if (entry.id == id) {
            return &entry;
        }
    }
    return nullptr;
}

const BuildableRegistry::Entry* BuildableRegistry::GetByIndex(size_t index) const {
    if (index >= m_Entries.size()) {
        return nullptr;
    }
    return &m_Entries[index];
}
