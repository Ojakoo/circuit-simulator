#include <iostream>
#include <sstream>
#include <iomanip>

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

void GUIComponent::DrawInfo(sf::RenderWindow &window) {
    sf::Font font;
    if (font.loadFromFile("../fonts/arial.ttf"))
    {
        sf::Text name(GetName(), font, 14);
        std::string unit;
        switch (GetType()) {
            case RESISTOR:
                unit = " OHM";
                break;
            case CAPACITOR:
                unit = " F";
                break;
            case INDUCTOR:
                unit = " H";
                break;
            case VOLTAGE_SOURCE:
                unit = " V";
                break;
            case CURRENT_SOURCE:
                unit = " A";
                break;
        }
        int precision = 1;
        if (GetValue() != 0.0 && GetValue() < 0.00001) {
            precision = 9;
        } else if (GetValue() != 0.0 && GetValue() < 0.1) {
            precision = 5;
        }
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << GetValue();
        sf::Text value(stream.str() + unit, font, 14);
        name.setFillColor(sf::Color::Black);
        value.setFillColor(sf::Color::Yellow);
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
    if (component_->GetTerminalNode(INPUT)) {
        auto rot = getRotation();
        auto bounds = getGlobalBounds();
        sf::Text in(GetTerminalNode(INPUT)->GetName(), font, 14);
        in.setFillColor(sf::Color::Blue);
        if (rot == 0)
            input_rect_.setPosition(bounds.left, bounds.top + bounds.height/2);
        else if (rot == 90)
            input_rect_.setPosition(bounds.left + bounds.width/2, bounds.top);
        else if (rot == 180)
            input_rect_.setPosition(bounds.left + bounds.width, bounds.top + bounds.height/2);
        else
            input_rect_.setPosition(bounds.left + bounds.width/2, bounds.top + bounds.height);
        window.draw(input_rect_);
        in.setPosition(input_rect_.getPosition());
        window.draw(in);
    }
    if (component_->GetTerminalNode(OUTPUT)) {
        auto rot = getRotation();
        auto bounds = getGlobalBounds();
        sf::Text out(GetTerminalNode(OUTPUT)->GetName(), font, 14);
        out.setFillColor(sf::Color::Blue);
        if (rot == 0)
            output_rect_.setPosition(bounds.left + bounds.width, bounds.top + bounds.height/2);
        else if (rot == 90)
            output_rect_.setPosition(bounds.left + bounds.width/2, bounds.top + bounds.height);
        else if (rot == 180)
            output_rect_.setPosition(bounds.left, bounds.top + bounds.height/2);
        else
            output_rect_.setPosition(bounds.left + bounds.width/2, bounds.top);
        window.draw(output_rect_);
        out.setPosition(output_rect_.getPosition());
        window.draw(out);
    }
}

void GUIComponent::ConnectNodeToTerminal(TerminalType terminal, std::shared_ptr<Node> node) {
    component_->ConnectNodeToTerminal(node, terminal);
}

std::shared_ptr<Node> GUIComponent::GetTerminalNode(TerminalType terminal) const {
    return component_->GetTerminalNode(terminal);
}

void GUIComponent::ConnectWire(TerminalType terminal) {
    connected_wires_[terminal] += 1;
}

void GUIComponent::RemoveWire(TerminalType terminal) {
    connected_wires_[terminal] -= 1;
    if (connected_wires_[terminal] <= 0) {
        // disconnect
        ConnectNodeToTerminal(terminal, nullptr);
        connected_wires_[terminal] = 0;
    }
}