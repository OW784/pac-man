#pragma once

#include "engine/game_system.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class MenuScene : public Scene {
private:
    sf::Text text;
    sf::Font font;

    sf::RectangleShape playButton;
    sf::RectangleShape quitButton;

    sf::Text playText;
    sf::Text quitText;
public:
    MenuScene() = default;

    void update(const float& dt) override;
    void render() override;
    void load() override;
};

class GameScene : public Scene {
private:
    sf::Text text;
    sf::Clock scoreClock;
    void respawn();

public:
    GameScene() = default;

    void update(const float& dt) override;
    void render() override;
    void load() override;

    std::shared_ptr<Entity> _player;
    std::vector<std::shared_ptr<Entity>> _ghosts;
};

namespace Scenes {
    extern std::shared_ptr<Scene> menu;
    extern std::shared_ptr<Scene> game;
}

