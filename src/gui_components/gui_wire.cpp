#include "node.hpp"

#include "gui_components/gui_wire.hpp"

#include <iostream>
#include <algorithm>

GUIWire::GUIWire() : sf::VertexArray(sf::LineStrip, 1) { }

GUIWire::~GUIWire() {
    // disconnect every component
    for ( auto it : components_ ) {
        for (auto comp : components_[it.first]) {
            comp->RemoveWire(it.first);
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
    for ( auto pair : components_) {
        for (auto comp : pair.second) {
            comp->ConnectNodeToTerminal(pair.first, node);
        }
    }
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

void GUIWire::SetConnPoint(sf::Vector2i point) {
    sf::RectangleShape c;
    c.setPosition(point.x, point.y);
    c.setOrigin(5, 5);
    c.setSize(sf::Vector2f(9, 9));
    c.setFillColor(sf::Color::Black);
    wire_conns_.push_back(c);
}

void GUIWire::DrawConns(sf::RenderWindow &window) {
    for ( auto conn : wire_conns_ ) {
        window.draw(conn);
    }
}