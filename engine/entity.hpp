#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// An individual game object
class Entity {
public:
    virtual ~Entity() = default;

    virtual void update(const float& dt) = 0;
    virtual void render() = 0;
};

// Manages all the entities in a scene
struct EntityManager {
    std::vector<std::shared_ptr<Entity>> list;

    void update(const float& dt);
    void render();
};