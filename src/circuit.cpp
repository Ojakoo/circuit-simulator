
#include "circuit.hpp"

const std::list<std::shared_ptr<Component>>& Circuit::GetComponents() const {
    return components_;
}

const MatrixXcf Circuit::AMatrix(float omega) const {

    //generate index map based on nodes
    std::map<std::string, int> indexes;

    int node_count = 0;

    for ( auto const& i : nodes_ ) {
        if ( i.second->GetType() != GROUND ) {
            indexes[ i.first ] = node_count;
            node_count++;
        }
    }

    MatrixXcf A = MatrixXcf::Zero(n_ + m_, n_ + m_);  // initialize A matrix

    for ( auto const& component : components_ ) {

        std::shared_ptr<Node> out = component->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = component->GetTerminalNode(INPUT);

        if (out == nullptr || in == nullptr) continue;  // other node is not connected

        ComponentType type = component->GetType();
        ComponentClass cls = component->GetClass();

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
                        admittance = std::complex<float>(
                            0, component->GetValue() * omega
                        );  // Y = 1 / Z = j*w*C
                        break;
                    case INDUCTOR:
                        admittance = std::complex<float>(
                            0, 1 / (component->GetValue() * omega)
                        );  // Y = 1 / Z = 1 / (j*w*L)
                        break;
                    default:
                        break;
                }

                if ( in->GetType() != GROUND && out->GetType() != GROUND ) {
                    // component is between two nodes
                    A( indexes[out_name], indexes[in_name] ) += -admittance;
                    A( indexes[in_name], indexes[out_name] ) += -admittance;
                }
                if ( out->GetType() != GROUND ) {
                    // output terminal is connected to some node
                    A( indexes[out_name], indexes[out_name] ) += admittance;
                }
                if ( in->GetType() != GROUND ) {
                    // input terminal is connected to some node
                    A( indexes[in_name], indexes[in_name] ) += admittance;
                }
                break;
            case ACTIVE:
                /*
                Construct B and C submatrix.
                */
                switch ( type ) {
                    case DC_VOLTAGE_SOURCE:
                    if ( out->GetType() != GROUND ) {
                        // output terminal is connected to some node
                        A(n_, indexes[out_name]) = 1;  // append value to B submatrix
                        A(indexes[out_name], n_) = 1;  // append value to C submatrix
                    }
                    if ( in->GetType() != GROUND ) {
                        // input terminal is connected to some node
                        A(n_, indexes[in_name]) = -1;  // append value to B submatrix
                        A(indexes[in_name], n_) = -1;  // append value to C submatrix
                    }
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    return A;
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