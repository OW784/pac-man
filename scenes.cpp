#include "scenes.hpp"
#include "engine/renderer.hpp"
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
    text.setString("Almost Pacman");
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Yellow);
    
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    text.setPosition(400.f, 170.f);
}

void MenuScene::update(const float& dt) {
    Scene::update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        GameSystem::set_active_scene(Scenes::game);
    }
}

void MenuScene::render() {
    
    Renderer::queue(&text);
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