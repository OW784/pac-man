#include "game_system.hpp"
#include "entity.hpp"
#include <SFML/graphics.hpp>
#include "renderer.hpp"

std::shared_ptr<Scene> GameSystem::_active_scene = nullptr;

sf::RenderWindow* GameSystem::_window = nullptr;

void resizeView(sf::RenderWindow& window, sf::View& view)
{
	float windowWidth = static_cast<float>(window.getSize().x);
	float windowHeight = static_cast<float>(window.getSize().y);

	float windowRatio = windowWidth / windowHeight;
	float viewRatio = 800.f / 600.f;

	float viewportWidth = 1.f;
	float viewportHeight = 1.f;
	float viewportLeft = 0.f;
	float viewportTop = 0.f;

	if (windowRatio > viewRatio)
	{
		viewportWidth = viewRatio / windowRatio;
		viewportLeft = (1.f - viewportWidth) / 2.f;
	}
	else
	{
		viewportHeight = windowRatio / viewRatio;
		viewportTop = (1.f - viewportHeight) / 2.f;
	}

	view.setViewport(sf::FloatRect(
		viewportLeft,
		viewportTop,
		viewportWidth,
		viewportHeight
	));

	window.setView(view);
}

void GameSystem::start(unsigned int width, unsigned int height,
	const std::string& name, const float& time_step) {
	sf::RenderWindow window(sf::VideoMode({ width, height }), name);
	_window = &window;
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
	resizeView(window, view);

	Renderer::initialise(window);

	_init();
	sf::Event event;
	while (window.isOpen()) {
		static sf::Clock clock;
		float dt = clock.restart().asSeconds();
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				resizeView(window, view);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
		window.clear();
		_update(dt);
		_render(window);
		sf::sleep(sf::seconds(time_step));

		window.display();

	}
	clean();
	_window = nullptr;
}

void GameSystem::_init() {

}

void GameSystem::_update(const float& dt) {
	if (_active_scene) {
		_active_scene->update(dt);
	}
}

void GameSystem::_render(sf::RenderWindow& window) {
	if (_active_scene) {
		_active_scene->render();
	}
	Renderer::render();
}

void GameSystem::clean() {
	if (_active_scene) {
		_active_scene->unload();
		_active_scene.reset();
	}

}

void GameSystem::set_active_scene(const std::shared_ptr<Scene>& act_sc) {
	_active_scene = act_sc;
}

void GameSystem::resize_window(unsigned int width, unsigned int height) {
	if (_window != nullptr) {

		// Resize the actual window
		_window->setSize(sf::Vector2u(width, height));

		// Update the view so the maze doesn't stretch
		sf::View view(
			sf::FloatRect(
				0.f,
				0.f,
				static_cast<float>(width),
				static_cast<float>(height)
			)
		);

		_window->setView(view);
	}
}


void Scene::update(const float& dt) {
	em.update(dt);
}

void Scene::render() {
	em.render();
}

void Scene::unload() {
	em.list.clear();
}

sf::RenderWindow* GameSystem::getWindow()
{
	return _window;
}