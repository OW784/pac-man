#include "renderer.hpp"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

sf::RenderWindow& Renderer::get_window() { return *rw; }

void Renderer::shutdown() {
    while (!sprites.empty())
        sprites.pop();
}

void Renderer::update(const float& dt) {}

void Renderer::render() {
    if (rw == nullptr) {
        throw("No render window set! ");
    }
    while (!sprites.empty()) {
        rw->draw(*sprites.front());
        sprites.pop();
    }
}

void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }