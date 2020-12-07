#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

#include "node.hpp"


class GUIGround : public sf::RectangleShape {
    public:
        GUIGround();
        virtual ~GUIGround();
        void draw(sf::RenderWindow &window);
        void SetNode(std::shared_ptr<Node> node);
        std::shared_ptr<Node> GetNode();
    private:
        sf::VertexArray lines;
        std::shared_ptr<Node> node_ = nullptr;
};