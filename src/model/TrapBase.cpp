#include "model/TrapBase.h"
#include <algorithm>
#include <utility>

TrapBase::TrapBase(const glm::vec2& position)
    : TowerBase(position) {
}

std::vector<HitEffectEvent> TrapBase::ConsumeHitEffectEvents() {
    std::vector<HitEffectEvent> events = std::move(m_HitEffectEvents);
    m_HitEffectEvents.clear();
    return events;
}

void TrapBase::AddHitEffectEvent(HitEffectEvent event) {
    m_HitEffectEvents.push_back(std::move(event));
}

bool TrapBase::OnRoundEnded() {
    if (m_ShouldRemove) {
        return false;
    }
    m_ShouldRemove = true;
    return true;
}

void TrapBase::Update(
    float,
    std::vector<std::shared_ptr<EnemyModel>>& enemies,
    std::vector<std::shared_ptr<ProjectileModel>>&
) {
    for (auto it = m_TriggeredEnemies.begin(); it != m_TriggeredEnemies.end();) {
        const EnemyModel* triggeredEnemy = *it;
        const bool enemyStillExists = std::any_of(
            enemies.begin(),
            enemies.end(),
            [triggeredEnemy](const std::shared_ptr<EnemyModel>& enemy) {
                return enemy && enemy.get() == triggeredEnemy && enemy->IsAlive();
            }
        );

        if (!enemyStillExists) {
            it = m_TriggeredEnemies.erase(it);
            continue;
        }
        ++it;
    }

    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->IsAlive()) {
            continue;
        }

        const float dist = glm::distance(enemy->GetPosition(), m_Position);
        if (dist <= m_TriggerRadius && !m_TriggeredEnemies.count(enemy.get())) {
            TriggerOn(enemy);
            m_TriggeredEnemies.insert(enemy.get());
        }
    }
}
//TODO: 陷阱的hitbox
