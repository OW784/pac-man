#include "Level_system.hpp"
#include <fstream>
#include <iostream>


std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
int LevelSystem::_width;
int LevelSystem::_height;
sf::Vector2f LevelSystem::_start_position{ 0.f, 0.f };
sf::Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tile_size(100.f);
std::vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colors{ {WALL, sf::Color::White}, {END, sf::Color::Red} };

int LevelSystem::get_height() { return _height; }
int LevelSystem::get_width() { return _width; }

sf::Color LevelSystem::get_color(LevelSystem::Tile t) {
    auto it = _colors.find(t);
    if (it == _colors.end()) {
        _colors[t] = sf::Color::Transparent;
    }
    return _colors[t];
}

void LevelSystem::set_color(LevelSystem::Tile t, sf::Color c) {
    _colors[t] = c;
}

void LevelSystem::load_level(const std::string& path, float tile_size) {
    _tile_size = tile_size;
    int w = 0, h = 0;
    std::string buffer;

    // Load in file to buffer
    std::ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else {
        throw std::string("Couldn't open level file: ") + path;
    }
    int x = 0;

    std::vector<Tile> temp_tiles;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        switch (c) {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 's':
            temp_tiles.push_back(START);
            _start_position = get_tile_position({ x,h });
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case '+':
            temp_tiles.push_back(WAYPOINT);
            break;
        case 'n':
            temp_tiles.push_back(ENEMY);
            break;
        case '\n':      // end of line
            if (w == 0) { // if we haven't written width yet
                w = i;      // set width
            }
            x = 0;
            h++; // increment height
            break;
        default:
            std::cout << c << std::endl; // Don't know what this tile type is
        }
        x++;
    }
    std::cout << "File: " << path << std::endl;
    std::cout << "Width: " << w << std::endl;
    std::cout << "Height: " << h << std::endl;
    std::cout << "Expected: " << w * h << std::endl;
    std::cout << "Actual: " << temp_tiles.size() << std::endl;
    if (temp_tiles.size() != (w * h)) {
        throw std::string("Can't parse level file") + path;
    }
    _tiles = std::make_unique<Tile[]>(w * h);
    _width = w; //set static class vars
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    std::cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    build_sprites();
}

void LevelSystem::build_sprites() {
    _sprites.clear();
    for (int y = 0; y < LevelSystem::get_height(); ++y) {
        for (int x = 0; x < LevelSystem::get_width(); ++x) {
            std::unique_ptr<sf::RectangleShape> s = std::make_unique<sf::RectangleShape>();
            s->setPosition(get_tile_position({ x, y }));
            s->setSize(sf::Vector2f(_tile_size, _tile_size));
            s->setFillColor(get_color(get_tile({ x, y })));
            _sprites.push_back(move(s));
        }
    }
}

sf::Vector2f LevelSystem::get_tile_position(sf::Vector2i p) {
    return (sf::Vector2f(p.x, p.y) * _tile_size);
}

LevelSystem::Tile LevelSystem::get_tile(sf::Vector2i p) {
    if (p.x > _width || p.y > _height) {
        throw std::string("Tile out of range: ") + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

LevelSystem::Tile LevelSystem::get_tile_at(sf::Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw std::string("Tile out of range ");
    }
    return get_tile(sf::Vector2i((v - _offset) / (_tile_size)));
}

void LevelSystem::render(sf::RenderWindow& window) {
    for (size_t i = 0; i < _width * _height; ++i) {
        window.draw(*_sprites[i]);
    }
}

sf::Vector2f LevelSystem::get_start_position() { return _start_position; }