#pragma once

#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include <memory>
#include <string>
#include <vector>

class Entity;


class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void update(const float& dt);
    virtual void render();
    virtual void load() = 0;
    virtual void unload();

    std::vector<std::shared_ptr<Entity>>& get_entities() {
        return em.list;
    }

protected:
    EntityManager em;
};

class GameSystem {
public:
    static void start(unsigned int width, unsigned int height,
        const std::string& name, const float& time_step);

    static void clean();
    static void set_active_scene(const std::shared_ptr<Scene>& act_sc);
    static void resize_window(unsigned int width, unsigned int height);

private:
    static void _init();
    static void _update(const float& dt);
    static void _render(sf::RenderWindow& window);

    static std::shared_ptr<Scene> _active_scene;
    static sf::RenderWindow* _window;
};