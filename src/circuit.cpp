#include "component.hpp"
#include "circuit.hpp"


const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const { 
    return components_; 
}

const int Circuit::GetNodeCount() const {
    // return number of real nodes in circuit (no ground node)
    return n_;
}

const int Circuit::GetSourceCount() const {
    return m_;
}


const std::shared_ptr<Node> Circuit::AddNode(const std::string& node_name) {
    auto it = nodes_.find(node_name);
    if (it == nodes_.end()) {
        if (node_name != "0") n_++;
        nodes_[node_name] = std::make_shared<Node>(node_name, node_name == "0" ? GROUND : NORMAL);
    }
    return nodes_[node_name];
}

void Circuit::AddComponent(std::shared_ptr<Component> component) {
    ComponentType type = component->GetType();
    if (type == DC_VOLTAGE_SOURCE || type == DC_CURRENT_SOURCE) {
        m_++;
    }
    components_.push_back(component); 
}

std::ostream &operator<<(std::ostream& out, const Circuit& circuit) {
    for (auto it : circuit.GetComponents()) {
        out << "\n" << *it;
    }
    return out.flush();
}