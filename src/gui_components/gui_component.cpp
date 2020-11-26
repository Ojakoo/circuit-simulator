#include <iostream>

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

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

        // init terminal rects
        input_rect_.setPosition(0,0);
        input_rect_.setSize(sf::Vector2f(9, 9));
        input_rect_.setOrigin(5, 5);
        input_rect_.setFillColor(sf::Color::Black);

        output_rect_.setPosition(0, 0);
        output_rect_.setSize(sf::Vector2f(9, 9));
        output_rect_.setOrigin(5, 5);
        output_rect_.setFillColor(sf::Color::Black);
     }

const std::string GUIComponent::GetName() const {
    return component_->GetName();
}

float GUIComponent::GetValue() const {
    return component_->GetValue();
}

const ComponentType GUIComponent::GetType() const {
    return component_->GetType();
}

const ComponentClass GUIComponent::GetClass() const {
    return component_->GetClass();
}

const std::shared_ptr<Component> GUIComponent::GetComponent() const {
    return component_;
}

void GUIComponent::SetValue(float newval) {
    component_->SetValue(newval);
}

void GUIComponent::Disconnect() {
    input_connected_ = false;
    output_connected_ = false;
}

void GUIComponent::DrawInfo(sf::RenderWindow &window) const {
    sf::Font font;
    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text name(GetName(), font, 14);
        sf::Text value(std::to_string(GetValue()), font, 14);
        name.setFillColor(sf::Color::Black);
        value.setFillColor(sf::Color::Black);
        auto rot = getRotation();
        auto bounds = getGlobalBounds();
        if ( rot == 90.f || rot == 270.f) {
            name.setPosition(bounds.left - 20, bounds.top + bounds.height/2);
            value.setPosition(bounds.left - 20, bounds.top + bounds.height/2 + 20);
        } else {
            name.setPosition(bounds.left + bounds.width/2, bounds.top - 20);
            value.setPosition(bounds.left + bounds.width/2 + 20, bounds.top - 20);
        }
        window.draw(name);
        window.draw(value);
    }
}

void GUIComponent::DrawTerminalRects(sf::RenderWindow &window) {
    if (input_connected_) {
        window.draw(input_rect_);
    }
    if (output_connected_) {
        window.draw(output_rect_);
    }
}

void GUIComponent::SetTerminalRects(TerminalType terminal, sf::Vector2f coords) {
    switch ( terminal ) {
        case INPUT:
            input_connected_ = true;
            input_rect_.setPosition(coords);
            break;
        case OUTPUT:
            output_connected_ = true;
            output_rect_.setPosition(coords);
            break;
    }
}
