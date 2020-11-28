#pragma once

#include <map>

#include "gui_components/gui_component.hpp"

#include <SFML/Graphics/VertexArray.hpp>


class GUIWire : public sf::VertexArray {
    public:
        GUIWire();
        ~GUIWire();

        void SetNodeType(NodeType type);

        const NodeType GetNodeType() const;

        void SetNode(std::shared_ptr<Node> node);

        const std::shared_ptr<Node> GetNode() const;

        void ConnectComponent(std::shared_ptr<GUIComponent> comp, TerminalType type);

    private:
        std::shared_ptr<Node> node_ = nullptr;
        std::map<std::shared_ptr<GUIComponent>, TerminalType> connected_;
};