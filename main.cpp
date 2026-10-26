#include "engine/game_system.hpp"
#include "scenes.hpp"
#include <memory>

int main() {
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::game = std::make_shared<GameScene>();

    Scenes::menu->load();
    Scenes::game->load();

    GameSystem::set_active_scene(Scenes::menu);

    GameSystem::start(800, 600, "Pac-Man", 1.0f / 60.0f);

    return 0;
}