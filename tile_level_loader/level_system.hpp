#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>

class LevelSystem {
public:
	enum Tile { EMPTY, START, END, WALL, ENEMY, WAYPOINT };

	static void load_level(const std::string&, float tile_size = 100.f);
	static void render(sf::RenderWindow& window);
	static sf::Color get_color(Tile t);
	static void set_color(Tile t, sf::Color c);

	static Tile get_tile(sf::Vector2i);
	static sf::Vector2f get_tile_position(sf::Vector2i);
	static std::vector<sf::Vector2i> find_tiles(Tile tile);
	static Tile get_tile_at(sf::Vector2f);
	static int get_height();
	static int get_width();
	static sf::Vector2f get_start_position();
	static void set_offset(const sf::Vector2f& offset);

protected:
	static std::unique_ptr<Tile[]> _tiles;
	static int _width;
	static int _height;
	static sf::Vector2f _offset;
	static float _tile_size;
	static std::map<Tile, sf::Color> _colors;
	static sf::Vector2f _start_position;
	static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;
	static void build_sprites();


private:
	LevelSystem() = delete;
	~LevelSystem() = delete;

};