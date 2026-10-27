#include "entity.hpp"


Entity::Entity()
    : _position(0.f, 0.f),
    _rotation(0.f),
    _alive(true),
    _visible(true),
    _for_deletion(false)
{
}



void Entity::update(const float& dt)
{
    if (!_alive)
        return;

    for (auto& component : _components)
    {
        component->update(dt);
    }
}

Component::Component(Entity* const p)
    : _parent(p), _for_deletion(false)
{
}

Component::~Component() = default;

void Entity::render()
{
    if (!_visible)
        return;

    for (auto& component : _components)
    {
        component->render();
    }
}

const sf::Vector2f& Entity::get_position() const
{
    return _position;
}

void Entity::set_position(const sf::Vector2f& pos)
{
    _position = pos;
}

void EntityManager::update(const float& dt)
{
    for (auto& entity : list)
    {
        entity->update(dt);
    }
}

void EntityManager::render()
{
    for (auto& entity : list)
    {
        entity->render();
    }
}