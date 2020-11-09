
#include "circuit.hpp"

<<<<<<< HEAD
const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const {
    return components_;
}

const MatrixXcf Circuit::sMatrix() const{
    //generate index map based on node map_
    std::map<std::string, int> indexes;

    int node_count = 0;

    for ( auto const& i : nodes_ ) {
        if ( i.second->GetType() != GROUND ) {
            indexes[ i.first ] = node_count;
            node_count++;
        }
    }

    int voltage_source_count;
    int current_source_count;

    MatrixXcf A = MatrixXcf::Zero(n_+m_,n_+m_);

    for ( auto const& i : components_ ) {
        ComponentType type = i->GetType();
        std::shared_ptr<Node> out = i->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = i->GetTerminalNode(INPUT);

        if (type == RESISTOR) {
            std::string out_name = out->GetName();
            std::string in_name = in->GetName();

            if ( out->GetType() != GROUND ) {
                A( indexes[out_name], indexes[out_name] ) += ( 1/( i->GetValue() ));
            }
            if ( in->GetType() != GROUND ) {
                A( indexes[in_name], indexes[in_name] ) += ( 1/( i->GetValue() ));
            }
            if ( in->GetType() != GROUND && out->GetType() != GROUND ) {
                A( indexes[out_name], indexes[in_name] ) += ( -1/( i->GetValue() ));
                A( indexes[in_name], indexes[out_name] ) += ( -1/( i->GetValue() ));
            }
        } else if (type == DC_VOLTAGE_SOURCE) {
            
        }
    }

    return A;
=======
const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const { 
    return components_; 
>>>>>>> circuit testing
}

const int Circuit::GetNodeCount() const {
    // return number of real nodes in circuit (no ground node)
    return n_;
}

const int Circuit::GetSourceCount() const {
    return i_;
}

const int Circuit::GetVoltageSourceCount() const {
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
        i_++;
        if (type == DC_VOLTAGE_SOURCE) {
            m_++;
        }
    }
    
    components_.push_back(component);
}

std::ostream &operator<<(std::ostream& out, const Circuit& circuit) {
    for (auto it : circuit.GetComponents()) {
        out << "\n" << *it;
    }
    return out.flush();
}