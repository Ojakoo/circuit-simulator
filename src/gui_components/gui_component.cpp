#include <iostream>

#include "SFML/Graphics/Text.hpp"

#include "gui_components/gui_component.hpp"


GUIComponent::GUIComponent(const std::string& texture, std::shared_ptr<Component> component)
    : sf::Sprite() {
        // set texture
        tx_ = sf::Texture();
        tx_.loadFromFile(texture);
        setTexture(tx_);

        // set component
        component_ = component;
        setOrigin(getLocalBounds().width/2, getLocalBounds().height/2);
     }

const std::string GUIComponent::GetName() const {
    return component_->GetName();
}

const ComponentType GUIComponent::GetType() const {
    return component_->GetType();
}

const std::shared_ptr<Component> GUIComponent::GetComponent() const {
    return component_;
}

void GUIComponent::DrawName(sf::RenderWindow &window) const {
    sf::Font MyFont;
    if (MyFont.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text text(GetName(), MyFont, 14);
        text.setFillColor(sf::Color::Black);
        auto rot = getRotation();
        auto bounds = getGlobalBounds();
        if ( rot == 90.f || rot == 270.f) {
            text.setPosition(bounds.left - 20, bounds.top + bounds.height/2);
        } else {
            text.setPosition(bounds.left + bounds.width/2, bounds.top - 20);
        }
        window.draw(text);
    }
}