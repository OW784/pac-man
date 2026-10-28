#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <typeindex>
#include <algorithm>

class Entity;

class Component {
public:
    Component() = delete;
    bool is_for_deletion() const;
    virtual void update(const float& dt) = 0;
    virtual void render() = 0;
    virtual ~Component();
protected:
    Entity* const _parent;
    bool _for_deletion; // should be removed
    explicit Component(Entity* const p);
};
// An individual game object
class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    virtual void update(const float& dt);
    virtual void render();

    const sf::Vector2f& get_position() const;
    void set_position(const sf::Vector2f& _position);
    bool is_for_deletion() const;
    float get_rotation() const;
    void set_rotation(float _rotation);
    bool is_alive() const;
    void set_alive(bool _alive);
    void set_for_delete();
    bool is_visible() const;
    void set_visible(bool _visible);

    template <typename T, typename... Targs>
    std::shared_ptr<T> add_component(Targs... params) {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
        _components.push_back(sp);
        return sp;
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> get_components() const {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (const auto c : _components) {
            if (typeid(*c) == typeid(T)) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return std::move(ret);
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> get_compatible_component() {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (auto c : _components) {
            auto dd = dynamic_cast<T*>(&(*c));
            if (dd) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return ret;
    }

protected:
    std::vector<std::shared_ptr<Component>> _components;
    sf::Vector2f _position;
    float _rotation;
    bool _alive;       // should be updated
    bool _visible;     // should be rendered
    bool _for_deletion; //
};

// Manages all the entities in a scene
struct EntityManager {
    std::vector<std::shared_ptr<Entity>> list;

    void update(const float& dt);
    void render();
};

