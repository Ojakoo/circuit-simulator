#include <iostream>

#include "component.hpp"


Component::Component(const std::string &name)
    : name_(name) { }


const std::string& Component::GetName() const {
    return name_;
}

std::ostream &operator<<(std::ostream &out, const Component& c) {
    ComponentType type = c.GetType();

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

    out << ". Name: " << c.GetName() << ", value: " << c.GetValue();

    return out.flush();
}