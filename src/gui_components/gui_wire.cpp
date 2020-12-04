#include "node.hpp"

#include "gui_components/gui_wire.hpp"

#include <iostream>
#include <algorithm>

GUIWire::GUIWire(Circuit &circuit) : circuit_(circuit), sf::VertexArray(sf::LineStrip, 1) { }

GUIWire::~GUIWire() {
    // disconnect every component
    for ( auto it : components_ ) {
        for (auto comp : components_[it.first]) {
            comp->RemoveWire(it.first, circuit_);
        }
    }
}

const NodeType GUIWire::GetNodeType() const {
    return node_->GetType();
}

void GUIWire::SetNodeType(NodeType type) {
    node_->SetNodeType(type);
}

void GUIWire::SetNode(std::shared_ptr<Node> node) {
    node_ = node;
}

const std::shared_ptr<Node> GUIWire::GetNode() const {
    return node_;
}

void GUIWire::ConnectComponent(std::shared_ptr<GUIComponent> comp, TerminalType type) {
    components_[type].push_back(comp);
}

std::map<TerminalType, std::vector<std::shared_ptr<GUIComponent>>> GUIWire::GetComponents() {
    return components_;
}

void GUIWire::DrawInfo(sf::RenderWindow &window) {
    if (node_ && getVertexCount() >= 2) {
        sf::Font font;
        if (font.loadFromFile("../fonts/arial.ttf"))
        {
            sf::Text name(node_->GetName(), font, 14);
            name.setFillColor(sf::Color::Blue);
            name.setPosition((*this)[0].position);
            window.draw(name);
        }
    }
}