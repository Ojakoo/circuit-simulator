#pragma once

#include <string>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


class GUIComponent : public sf::Sprite {
    
    /*
    Base class for all GUI components.
    */

    public:
        GUIComponent(const std::string &texture);

    private:
        sf::Texture tx_;  // holds pointer for texture
};