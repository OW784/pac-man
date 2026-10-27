#include "components.hpp"

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
    /***
    if (valid_move(new_pos)) {
        _parent->set_position(new_pos);
    } ***/
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