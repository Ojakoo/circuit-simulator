#include "circuit.hpp"

Circuit::~Circuit() {
    for (auto it : components_) {
        delete it;
    }
}

const std::list<Component*>& Circuit::GetComponents() const { 
    return components_; 
}

void Circuit::AddComponent(Component* component) { 
    components_.push_back(component); 
}

std::ostream &operator<<(std::ostream& out, const Circuit& circuit) {
    for (auto it : circuit.GetComponents()) {
        out << "\n" << *it;
    }
    return out.flush();
}