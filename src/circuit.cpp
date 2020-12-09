#include <set>
#include <string>

#include "circuit.hpp"

const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const {
    return components_;
}

void Circuit::RemoveComponent(std::shared_ptr<Component> component) {
    components_.remove(component);
}

void Circuit::RemoveUnnecessaryNodes() {
    std::set<std::string> used_nodes;
    for ( auto comp : components_ ) {
        if (comp->GetTerminalNode(INPUT)) {
            used_nodes.insert(comp->GetTerminalNode(INPUT)->GetName());
        }
        if (comp->GetTerminalNode(OUTPUT)) {
            used_nodes.insert(comp->GetTerminalNode(OUTPUT)->GetName());
        }
    }
    auto itr = nodes_.begin();
    while (itr != nodes_.end()) {
        auto found = used_nodes.find((*itr).first);
        if ( found == used_nodes.end() ) {
            nodes_.erase(itr++);
        } else {
            ++itr;
        }
    }
}

void Circuit::ConstructMatrices() {
    RemoveUnnecessaryNodes();
    //generate index map based on nodes
    // works only on circuits with ac or dc sources only
    node_indexes_.clear();
    voltage_source_indexes_.clear();
    inductor_indexes_.clear();

    int inductor_sc_count = 0;
    int l = 0;  // inductor count
    int m = 0; // voltage source count
    int n = 0; // node count
    int voltage_sources_count = 0;

    float omega = omega_;

    for ( auto const& i : nodes_ ) {
        if ( i.second->GetType() != GROUND ) {
            node_indexes_[ i.first ] = n;
            n++;
        }
    }

    // count inductors and voltage sources
    for ( auto it : components_ ) {
        if ( it->GetType() == INDUCTOR && omega == 0) {
            inductor_indexes_[it->GetName()] = l + n;
            l += 1;
        }
        if ( it->GetType() == VOLTAGE_SOURCE ) {
            m += 1;
        }
    }

    A_ = MatrixXcf::Zero(n + m + l, n + m + l);  // initialize A matrix
    z_ = VectorXf::Zero(m + n + l);  // initialize z vector

    for ( auto const& component : components_ ) {

        std::shared_ptr<Node> out = component->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = component->GetTerminalNode(INPUT);

        if (out == nullptr || in == nullptr) continue;  // other node is not connected

        ComponentType type = component->GetType();
        ComponentClass cls = component->GetClass();
        std::string name = component->GetName();

        std::string out_name = out->GetName();
        std::string in_name = in->GetName();

        std::complex<float> admittance;

        switch ( cls ) {
            case PASSIVE:
                /*
                Construct G submatrix.
                */
                switch ( type ) {
                    case RESISTOR:
                        admittance = std::complex<float>(
                            1 / component->GetValue(), 0
                        );  // Y = 1 / Z = 1 / R
                        break;
                    case CAPACITOR:
                        if (omega != 0) {
                            admittance = std::complex<float>(
                                0, (component->GetValue() * omega)
                            );  // Y = 1 / Z = j*w*C
                        }
                        break;
                    case INDUCTOR:
                        if (omega != 0) {
                           admittance = std::complex<float>(
                                0, (- 1 / (component->GetValue() * omega))
                            );  // Y = 1 / Z = -j / (w*L) 
                        }
                        break;
                    default:
                        break;
                }
                if ( in->GetType() != GROUND && out->GetType() != GROUND ) {
                    // component is between two nodes
                    A_( node_indexes_[out_name], node_indexes_[in_name] ) += -admittance;
                    A_( node_indexes_[in_name], node_indexes_[out_name] ) += -admittance;
                }
                if ( out->GetType() != GROUND ) {
                    // output terminal is connected to some node
                    A_( node_indexes_[out_name], node_indexes_[out_name] ) += admittance;
                }
                if ( in->GetType() != GROUND ) {
                    // input terminal is connected to some node
                    A_( node_indexes_[in_name], node_indexes_[in_name] ) += admittance;
                }

                if ( type == INDUCTOR && omega == 0) {
                    int idx = inductor_indexes_[name];
                    
                    //A_( idx, idx ) = cd(0, (- 1 / (component->GetValue() * omega)));
                    if ( out->GetType() != GROUND ) {
                        A_( node_indexes_[out_name], idx ) = 1;
                        A_( idx, node_indexes_[out_name]) = 1;
                    }
                    if ( in->GetType() != GROUND ) {
                        A_( node_indexes_[in_name], idx ) = -1;
                        A_( idx, node_indexes_[in_name]) = -1;
                    }
                }

                break;
            case ACTIVE:
                /*
                Construct B and C submatrices and z vector.
                */
                switch ( type ) {
                    case VOLTAGE_SOURCE:
                        if ( voltage_source_indexes_.find(name) == voltage_source_indexes_.end() ) {
                            //if voltage source is not mapped map it for future reference
                            voltage_source_indexes_[name] = n + l + voltage_sources_count;
                            voltage_sources_count++;
                        }
                        if ( out->GetType() != GROUND ) {
                            // output terminal is connected to some node
                            A_(voltage_source_indexes_[name], node_indexes_[out_name]) = 1;  // append value to B submatrix
                            A_(node_indexes_[out_name], voltage_source_indexes_[name]) = 1;  // append value to C submatrix
                        }
                        if ( in->GetType() != GROUND ) {
                            // input terminal is connected to some node
                            A_(voltage_source_indexes_[name], node_indexes_[in_name]) = -1;  // append value to B submatrix
                            A_(node_indexes_[in_name], voltage_source_indexes_[name]) = -1;  // append value to C submatrix
                        }
                        z_(voltage_source_indexes_[name]) = component->GetValue();    
                        break;
                    case CURRENT_SOURCE:
                        if ( out->GetType() != GROUND ) {
                            z_(node_indexes_[out_name]) += component->GetValue();
                        }
                        if ( in->GetType() != GROUND ) {
                            z_(node_indexes_[in_name]) += -component->GetValue();
                        }    
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

const std::shared_ptr<Node> Circuit::AddNode(const std::string& node_name) {
    auto it = nodes_.find(node_name);
    if (it == nodes_.end()) {
        nodes_[node_name] = std::make_shared<Node>(node_name, node_name == "0" ? GROUND : NORMAL);
    }
    return nodes_[node_name];
}

const std::shared_ptr<Node> Circuit::AddNode() {
    // completely new node
    std::set<std::string> node_names;
    for ( auto pair : nodes_ ) {
        node_names.insert(pair.first);
    }
    int j = 0;
    while ( j < 99999 ) {
        std::string s = "N" + std::to_string(j);
        auto it = node_names.find(s);
        if ( it == node_names.end() ) {
            // not in nodes
            auto n = std::make_shared<Node>(s);
            nodes_[s] = n;
            return n;
        }
        j++;
    }
    return nullptr;
}

void Circuit::RemoveNode(const std::string& node_name) {
    nodes_.erase(node_name);
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
