#include "entity.hpp"

void EntityManager::update(const float& dt) {
    for (auto& ent : list) {
        ent->update(dt);
    }
}

void EntityManager::render() {
    for (auto& ent : list) {
        ent->render();
    }
}