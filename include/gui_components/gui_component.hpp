#pragma once

#include <string>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "component.hpp"

class GUIComponent : public sf::Sprite {
    
    /*
    Base class for all GUI components.
    */

    public:
        GUIComponent(const std::string &texture, std::shared_ptr<Component> component);

        const std::string GetName() const;

        const ComponentType GetType() const;

        const std::shared_ptr<Component> GetComponent() const;

    private:
        sf::Texture tx_;  // holds pointer for texture
        std::shared_ptr<Component> component_;
};