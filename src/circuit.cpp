
#include "circuit.hpp"

const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const {
    return components_;
}

void Circuit::RemoveComponent(std::shared_ptr<Component> component) {
    components_.remove(component);
}

void Circuit::ConstructMatrices() {
    //generate index map based on nodes
    // works only on circuits with ac or dc sources only
    node_indexes_.clear();
    voltage_source_indexes_.clear();
    inductor_sc_indexes_.clear();

    int node_count = 0;
    int inductor_sc_count = 0;
    int voltage_sources_count = 0;

    float omega = omega_;

    for ( auto const& i : nodes_ ) {
        if ( i.second->GetType() != GROUND ) {
            node_indexes_[ i.first ] = node_count;
            node_count++;
        }
    }

    for ( auto const& component : components_ ) {
        ComponentType type = component->GetType();
        if ( type == AC_VOLTAGE_SOURCE || type == DC_VOLTAGE_SOURCE ) {
            std::string name = component->GetName();
            voltage_source_indexes_[name] = node_count + voltage_sources_count;
            voltage_sources_count++;
        } else if ( type == (INDUCTOR) && omega == 0 ) {
            std::string name = component->GetName();
            inductor_sc_indexes_[name] = node_count + m_ + inductor_sc_count;
            inductor_sc_count++;
        }
    }

    A_ = MatrixXcf::Zero(n_ + m_ + inductor_sc_count, n_ + m_ + inductor_sc_count);  // initialize A matrix
    z_ = VectorXf::Zero(m_ + n_ + inductor_sc_count);  // initialize z vector

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
                        } else if ( omega == 0 ) {
                            // Add short circuit for every inductor when handling DC matrix
                            if ( out->GetType() != GROUND ) {
                                A_( node_indexes_[out_name], inductor_sc_indexes_[name] ) += -1;
                                A_( inductor_sc_indexes_[name], node_indexes_[out_name] ) += -1;
                            }
                            if ( in->GetType() != GROUND ) {
                                A_( node_indexes_[in_name], inductor_sc_indexes_[name] ) += 1;
                                A_( inductor_sc_indexes_[name], node_indexes_[in_name] ) += 1;
                            }
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
                break;
            case ACTIVE:
                /*
                Construct B and C submatrices and z vector.
                */
                switch ( type ) {
                    case AC_VOLTAGE_SOURCE:
                    case DC_VOLTAGE_SOURCE:
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
                    case AC_CURRENT_SOURCE:
                    case DC_CURRENT_SOURCE:
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

const MatrixXcf Circuit::GetAMatrix() const {
    return A_;
}

const VectorXf Circuit::GetZMatrix() const {
    return z_;
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

const std::map<std::string, int> Circuit::GetNodeIndexes() const {
    return node_indexes_;
}

const std::map<std::string, int> Circuit::GetVoltageSourceIndexes() const {
    return voltage_source_indexes_;
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
    if ( (type == DC_VOLTAGE_SOURCE || type == AC_VOLTAGE_SOURCE) || (type == DC_CURRENT_SOURCE || type == AC_CURRENT_SOURCE)   ) {
        i_++;
        if (type == DC_VOLTAGE_SOURCE || type == AC_VOLTAGE_SOURCE ) {
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