#include "circuit.hpp"



const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const { 
    return components_; 
}

void Circuit::AddComponent(std::shared_ptr<Component> component) { 
    components_.push_back(component); 
}

std::ostream &operator<<(std::ostream& out, const Circuit& circuit) {
    for (auto it : circuit.GetComponents()) {
        out << "\n" << *it;
    }
    return out.flush();
}