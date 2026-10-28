#include "scenes.hpp"
#include "parameters.hpp"
#include "engine/renderer.hpp"
#include "engine/game_system.hpp"
#include "engine/entity.hpp"
#include "components.hpp"
#include <iostream>
#include "tile_level_loader/level_system.hpp"

using ls = LevelSystem;

std::shared_ptr<Scene> Scenes::menu = nullptr;
std::shared_ptr<Scene> Scenes::game = nullptr;

// MENU SCENE

void MenuScene::load() {
    if (!font.loadFromFile("assets/Fonts/DejaVuSans.ttf")) {
        std::cout << "ERROR: Font failed to load!\n";
    }
    else {
        std::cout << "Font loaded successfully!\n";
    }

    text.setFont(font);
    text.setString("ALMOST PACMAN");
    text.setCharacterSize(120);
    text.setScale(0.5f, 0.5f);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(255, 220, 40));

    text.setOutlineColor(sf::Color(20, 35, 100));
    text.setOutlineThickness(3.f);
    
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    text.setPosition(400.f, 170.f);

    // PLAY BUTTON
    playButton.setSize(sf::Vector2f(220.f, 60.f));
    playButton.setPosition(290.f, 300.f);
    playButton.setFillColor(sf::Color(25, 65, 180));
    playButton.setOutlineColor(sf::Color(90, 160, 255));
    playButton.setOutlineThickness(3.f);

    playText.setFont(font);
    playText.setString("PLAY");
    playText.setCharacterSize(56);
    playText.setScale(0.5f, 0.5f);
    playText.setFillColor(sf::Color::White);

    sf::FloatRect playBounds = playText.getLocalBounds();

    playText.setOrigin(
        playBounds.left + playBounds.width / 2.f,
        playBounds.top + playBounds.height / 2.f
    );

    playText.setPosition(400.f, 330.f);

    // QUIT BUTTON
    quitButton.setSize(sf::Vector2f(220.f, 60.f));
    quitButton.setPosition(290.f, 385.f);
    quitButton.setFillColor(sf::Color(207, 52, 52));
    quitButton.setOutlineColor(sf::Color(227, 92, 209));
    quitButton.setOutlineThickness(3.f);

    quitText.setFont(font);
    quitText.setString("QUIT");
    quitText.setCharacterSize(56);
    quitText.setScale(0.5f, 0.5f);
    quitText.setFillColor(sf::Color::White);

    sf::FloatRect quitBounds = quitText.getLocalBounds();

    quitText.setOrigin(
        quitBounds.left + quitBounds.width / 2.f,
        quitBounds.top + quitBounds.height / 2.f
    );

    quitText.setPosition(400.f, 415.f);
}

void MenuScene::update(const float& dt) {
    Scene::update(dt);

    sf::RenderWindow* window = GameSystem::getWindow();

    sf::Vector2f mousePos =
        window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    bool overPlay = playButton.getGlobalBounds().contains(mousePos);
    bool overQuit = quitButton.getGlobalBounds().contains(mousePos);

    playButton.setFillColor(
        overPlay ? sf::Color(45, 95, 230) : sf::Color(25, 65, 180)
    );

    quitButton.setFillColor(
        overQuit ? sf::Color(230, 75, 75) : sf::Color(207, 52, 52)
    );

    static bool wasPressed = false;
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (isPressed && !wasPressed)
    {
        if (overPlay)
        {
            GameSystem::set_active_scene(Scenes::game);
        }
        else if (overQuit)
        {
            window->close();
        }
    }
 
}

void MenuScene::render() {
    
    Renderer::queue(&text);

    Renderer::queue(&playButton);
    Renderer::queue(&playText);

    Renderer::queue(&quitButton);
    Renderer::queue(&quitText);
    
    Scene::render();
}

// GAME SCENE

void GameScene::load() {
    {
        ls::set_offset({ 120.f, 10.f });
        ls::set_color(ls::WALL, sf::Color(0, 40, 255));
        ls::load_level("assets/map/pacman.txt", 20.0f);  

        std::shared_ptr<Entity> player = std::make_shared<Entity>();
        _player = player;
        auto shape = player->add_component<ShapeComponent>();
        player->add_component<PlayerMovementComponent>();

        shape->set_shape<sf::CircleShape>(param::entity_size);
        shape->get_shape().setFillColor(sf::Color::Yellow);
        shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));
        em.list.push_back(player);
    }
    const sf::Color ghost_cols[]{ {208, 62, 25},    // red Blinky
                               {219, 133, 28},   // orange Clyde
                               {70, 191, 238},   // cyan Inky
                               {234, 130, 229} };


    for (int i = 0; i < param::ghost_count; i++) {
        std::shared_ptr<Entity> ghost = std::make_shared<Entity>();
        _ghosts.push_back(ghost);
        std::shared_ptr<ShapeComponent> shape = ghost->add_component<ShapeComponent>();
        ghost->add_component<EnemyAiComponent>();

        shape->set_shape<sf::CircleShape>(param::entity_size);
        shape->get_shape().setFillColor(ghost_cols[i % param::ghost_count]);
        shape->get_shape().setOrigin(
            sf::Vector2f(param::entity_size, param::entity_size));
        em.list.push_back(ghost);
    }
    respawn();
}


void GameScene::update(const float& dt) {
    Scene::update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        GameSystem::set_active_scene(Scenes::menu);
    }
}

void GameScene::render() {
    ls::render(Renderer::get_window());
    Scene::render();
}

void GameScene::respawn() {
    _player->set_position(ls::get_start_position());

    _player->get_compatible_component<ActorMovementComponent>()[0]
        ->set_speed(param::player_speed);

    std::vector<sf::Vector2i> ghost_spawns =
        ls::find_tiles(ls::ENEMY);

    for (size_t i = 1; i < em.list.size(); i++) {
        std::shared_ptr<Entity>& ghost = em.list[i];

        ghost->set_position(
            ls::get_tile_position(
                ghost_spawns[rand() % ghost_spawns.size()]
            )
        );

        ghost->get_compatible_component<ActorMovementComponent>()[0]
            ->set_speed(param::ghost_speed);
    }
}