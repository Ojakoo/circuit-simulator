#include "node.hpp"

#include "gui_components/gui_wire.hpp"

#include <iostream>

GUIWire::GUIWire() : sf::VertexArray(sf::LineStrip, 1) { }

GUIWire::~GUIWire() {
    // disconnect every component
    for ( auto it : connected_ ) {
        auto component = it.first->GetComponent();
        component->ConnectNodeToTerminal(nullptr, it.second);
        
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
    connected_[comp] = type;
}