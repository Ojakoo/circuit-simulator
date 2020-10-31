#pragma once

#include "node.hpp"


enum ComponentType {
    RESISTOR,
    INDUCTOR,
    CAPACITOR,
    DC_VOLTAGE_SOURCE,
    DC_CURRENT_SOURCE
};


enum TerminalType {
    POSITIVE,
    NEGATIVE
};


class Component {

    /*

    Base class for all circuit elements.

    Sources, resistors, capacitors and inductors
    all have unique name and the element might be connected
    to nodes via positive and negative terminals.

    */

    public:
        Component(
            const std::string& name,
            Node *pos_terminal = nullptr,
            Node *neg_terminal = nullptr
        );

        const std::string& GetName() const;

        const Node* GetTerminalNode(TerminalType terminal) const;

        void ConnectNodeToTerminal(Node *node, TerminalType terminal);

        virtual float GetValue() const = 0;

        virtual ComponentType GetType() const = 0;

        friend std::ostream &operator<<(std::ostream &out, const Component& c);

    private:
        std::string name_;
        Node* pos_terminal_;
        Node* neg_terminal_;
};