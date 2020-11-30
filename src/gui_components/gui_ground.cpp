#include "gui_components/gui_ground.hpp"

GUIGround::GUIGround() : sf::RectangleShape(sf::Vector2f(40.f, 40.f)) {
    lines = sf::VertexArray(sf::Lines, 8);
    lines[0].color = sf::Color::Black;
    lines[1].color = sf::Color::Black;
    lines[2].color = sf::Color::Black;
    lines[3].color = sf::Color::Black;
    lines[4].color = sf::Color::Black;
    lines[5].color = sf::Color::Black;
    lines[6].color = sf::Color::Black;
    lines[7].color = sf::Color::Black;
    setOrigin(20, 0);
}

void GUIGround::draw(sf::RenderWindow &window) {
    auto bounds = getGlobalBounds();
    lines[0].position = sf::Vector2f(bounds.left + 20, bounds.top + 0);
    lines[1].position = sf::Vector2f(bounds.left + 20, bounds.top + 20);
    lines[2].position = sf::Vector2f(bounds.left + 0, bounds.top + 20);
    lines[3].position = sf::Vector2f(bounds.left + 40, bounds.top + 20);
    lines[4].position = sf::Vector2f(bounds.left + 10, bounds.top + 30);
    lines[5].position = sf::Vector2f(bounds.left + 30, bounds.top + 30);
    lines[6].position = sf::Vector2f(bounds.left + 15, bounds.top + 40);
    lines[7].position = sf::Vector2f(bounds.left + 25, bounds.top + 40);
    window.draw(lines);
}