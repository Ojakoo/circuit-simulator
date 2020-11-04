#include <iostream>

#include "component.hpp"


Component::Component(const std::string &name, Node *pos_terminal, Node *neg_terminal)
    : name_(name), neg_terminal_(neg_terminal), pos_terminal_(pos_terminal) { }


const std::string& Component::GetName() const {
    return name_;
}

const Node* Component::GetTerminalNode(TerminalType terminal) const {
    switch ( terminal ) {
        case POSITIVE:
            return pos_terminal_;
        case NEGATIVE:
            return neg_terminal_;
        default:
            return nullptr;
    }
}

void Component::ConnectNodeToTerminal(Node *node, TerminalType terminal) {
    switch ( terminal ) {
        case POSITIVE:
            pos_terminal_ = node;
            break;
        case NEGATIVE:
            neg_terminal_ = node;
            break;
    }
}

std::ostream &operator<<(std::ostream &out, const Component& c) {
    ComponentType type = c.GetType();
    const Node *pos_terminal = c.GetTerminalNode(POSITIVE);
    const Node *neg_terminal = c.GetTerminalNode(NEGATIVE); 

    out << "Component type: ";

    switch ( type ) {
        case RESISTOR:
            out << "Resistor";
            break;
        case INDUCTOR:
            out << "Inductor";
            break;
        case CAPACITOR:
            out << "Capacitor";
            break;
        case DC_VOLTAGE_SOURCE:
            out << "DC Voltage Source";
            break;
        case DC_CURRENT_SOURCE:
            out << "DC Current Source";
            break;
        default:
            out << "Invalid component";
    }

    out << ". Name: "
        << c.GetName()
        << ", value: "
        << c.GetValue();

    // if positive or negative terminal is nullptr it means it's not connected
    out << ", +: " <<  (pos_terminal ? pos_terminal->GetName() : "NOT CONNECTED");
    out << ", -: " <<(neg_terminal ? neg_terminal->GetName() : "NOT CONNECTED");

    return out.flush();
}