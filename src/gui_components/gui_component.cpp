#include "gui_components/gui_component.hpp"


GUIComponent::GUIComponent(const std::string& texture)
    : sf::Sprite() {
        // set texture
        this->tx_ = sf::Texture();
        this->tx_.loadFromFile(texture);
        this->setTexture(this->tx_);
     }