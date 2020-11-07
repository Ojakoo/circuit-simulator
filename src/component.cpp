#include <iostream>

#include "component.hpp"


Component::Component(const std::string &name, std::shared_ptr<Node> input, std::shared_ptr<Node> output)
    : name_(name), input_(input), output_(output) { }


const std::string& Component::GetName() const {
    return name_;
}

const std::shared_ptr<Node> Component::GetTerminalNode(TerminalType terminal) const {
    switch ( terminal ) {
        case OUTPUT:
            return output_;
        case INPUT:
            return input_;
        default:
            return nullptr;
    }
}

void Component::ConnectNodeToTerminal(std::shared_ptr<Node> node, TerminalType terminal) {
    switch ( terminal ) {
        case OUTPUT:
            output_ = node;
            break;
        case INPUT:
            input_ = node;
            break;
    }
}

std::ostream &operator<<(std::ostream &out, const Component& c) {
    ComponentType type = c.GetType();
    std::shared_ptr<Node> output = c.GetTerminalNode(OUTPUT);
    std::shared_ptr<Node> input = c.GetTerminalNode(INPUT); 

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

    // if input or output terminal is nullptr it means it's not connected
    out << ", +: " <<  (output ? output->GetName() : "NOT CONNECTED");
    out << ", -: " <<(input ? input->GetName() : "NOT CONNECTED");

    return out.flush();
}