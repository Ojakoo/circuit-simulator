#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "SFML/Graphics/RectangleShape.hpp"

#include "component.hpp"


class GUIComponent : public sf::Sprite {
    
    /*
    Base class for all GUI components.
    */

    public:
        GUIComponent(const std::string &texture, std::shared_ptr<Component> component);

        const std::string GetName() const;

        float GetValue() const;

        const ComponentClass GetClass() const;

        void SetValue(float newval);

        const ComponentType GetType() const;

        const std::shared_ptr<Component> GetComponent() const;

        void DrawInfo(sf::RenderWindow &window) const;

        void DrawTerminalRects(sf::RenderWindow &window);

        void SetTerminalRects(TerminalType terminal, sf::Vector2f coords);

        void Disconnect();
        

    private:
        sf::Texture tx_;  // holds pointer for texture
        std::shared_ptr<Component> component_;
        bool input_connected_ = false;
        bool output_connected_ = false;
        sf::RectangleShape input_rect_;
        sf::RectangleShape output_rect_;
};