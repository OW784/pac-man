#pragma once

#include "engine/entity.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <renderer.hpp>

class ShapeComponent : public Component {
public:
    ShapeComponent() = delete;
    explicit ShapeComponent(Entity* const p);

    void update(const float& dt) override;
    void render() override;

    sf::Shape& get_shape() const;

    template <typename T, typename... Targs>
    void set_shape(Targs... params) {
        _shape.reset(new T(params...));
    }
protected:
    std::shared_ptr<sf::Shape> _shape;
};


class ActorMovementComponent : public Component {
public:
    explicit ActorMovementComponent(Entity* p);
    ActorMovementComponent() = delete;

    float get_speed() const;
    void set_speed(float _speed);

    void move(const sf::Vector2f&);
    void move(float x, float y);

    void render() override {}
    void update(const float& dt) override;
protected:
    bool _valid_move(const sf::Vector2f&);
    float _speed;
};

class PlayerMovementComponent : public ActorMovementComponent {
public:
    PlayerMovementComponent(Entity* p);
    void update(const float& dt) override;
};