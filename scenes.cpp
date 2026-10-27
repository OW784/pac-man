#include "scenes.hpp"
#include "engine/renderer.hpp"
#include "engine/game_system.hpp"
#include <iostream>

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
    // Add Pac-Man gameplay here later
}

void GameScene::update(const float& dt) {
    Scene::update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        GameSystem::set_active_scene(Scenes::menu);
    }
}

void GameScene::render() {
    Scene::render();
}