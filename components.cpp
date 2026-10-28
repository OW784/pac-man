#include "components.hpp"
#include "tile_level_loader/level_system.hpp"
#include "engine/parameters.hpp"
#include <iostream>



static const sf::Vector2i directions[] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0} };

using ls = LevelSystem;

void ShapeComponent::update(const float& dt) {
	_shape->setPosition(_parent->get_position());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::get_shape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}

void ActorMovementComponent::update(const float& dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {
}


/***
bool ActorMovementComponent::_valid_move(const sf::Vector2f& pos) {
    return (ls::get_tile_at(pos) != ls::WALL);
}
***/

void ActorMovementComponent::move(const sf::Vector2f& p) {
    sf::Vector2f new_pos = _parent->get_position() + p;
    _parent->set_position(new_pos);
}


void ActorMovementComponent::move(float x, float y) {
    move(sf::Vector2f(x, y));
}

float ActorMovementComponent::get_speed() const { return _speed; }

void ActorMovementComponent::set_speed(float speed) { _speed = speed; }

PlayerMovementComponent::PlayerMovementComponent(Entity* p)
    : ActorMovementComponent(p)
{
}


void PlayerMovementComponent::update(const float& dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        move(0.f, -get_speed() * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        move(-get_speed() * dt, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        move(0.f, get_speed() * dt);   
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        move(get_speed() * dt, 0.f);
    }
    
}

EnemyAiComponent::EnemyAiComponent(Entity* p)
    : ActorMovementComponent(p)
{
    _state = ROAMING;
    _direction = { 1.f, 0.f };
}

void EnemyAiComponent::update(const float& dt) {
    // amount to move
    const float mva = static_cast<float>(dt * _speed);

    // Current position
    const sf::Vector2f pos = _parent->get_position();

    // Next position
    const sf::Vector2f newpos = pos + _direction * mva;

    // Inverse of our current direction
    const sf::Vector2i baddir = -1 * sf::Vector2i(_direction);

    // Random new direction
    sf::Vector2i newdir = directions[(rand() % 4)];

    sf::Vector2f candidate =
        pos + sf::Vector2f(newdir) * mva;

    switch (_state) {
    case ROAMING:
        if (ls::get_tile_at(newpos) == ls::WALL ||
            ls::get_tile_at(pos) == ls::WAYPOINT) {
            _state = ROTATING;
        }
        else {
            move(_direction * mva);
        }
        break;

    case ROTATING:
        while (
            newdir == baddir ||
            ls::get_tile_at(
                pos + sf::Vector2f(newdir) * 20.f
            ) == ls::WALL
            ) {
            newdir = directions[rand() % 4];
        }

        _direction = sf::Vector2f(newdir);
        _state = ROTATED;
        break;

    case ROTATED:
        if (ls::get_tile_at(pos) != ls::WAYPOINT) {
            _state = ROAMING;
        }

        move(_direction * mva);
        break;
    }

    ActorMovementComponent::update(dt);
}