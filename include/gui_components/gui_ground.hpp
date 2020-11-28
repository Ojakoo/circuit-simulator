#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


class GUIGround : public sf::RectangleShape {
    public:
        GUIGround();
        virtual ~GUIGround() = default;
        void draw(sf::RenderWindow &window);
    private:
        sf::VertexArray lines;
};