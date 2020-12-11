
#include "MNAsolver.hpp"

MNAsolver::MNAsolver(){}

void MNAsolver::solveSteady(
        const MatrixXcf& A, 
        const VectorXcf& z,
        float omega, 
        std::map<std::string, int> node_indexes, 
        std::map<std::string, int> voltage_source_indexes, 
        std::map<std::string, int> inductor_indexes 
    ) {

    x_ = A.inverse()*z;

    node_voltages_.clear();
    voltage_source_currents_.clear();
    passive_component_currents_.clear();

    for ( auto const& i : node_indexes ) {
        node_voltages_[ i.first ] = x_( i.second );
    }
    for ( auto const& i : voltage_source_indexes ) {
        voltage_source_currents_[ i.first ] = x_( i.second );
    }
    for ( auto const& i : inductor_indexes ) {
        passive_component_currents_[ i.first ] = x_( i.second );
    }
}

std::ostream &operator<<(std::ostream& out, const MNAsolver& solver) {
    out << "\n\nnode voltages";
    for ( auto it : solver.GetNodeVoltages() ) {
        out << "\n" << it.first << " " << it.second;
    }
    out << "\n\nvoltage source currents";
    for ( auto it : solver.GetVoltageSourceCurrents() ) {
        out << "\n" << it.first << " " << it.second;
    }
    return out.flush();
}

void MNAsolver::setCurrents( const std::list<std::shared_ptr<Component>> components, float omega ) {
    // int omega = 0.0; //for ac circuit

    std::map<std::pair<std::string, std::string>,std::list<std::shared_ptr<Component>>> parallel_components_map;

    for ( auto const& component : components ) {
        std::shared_ptr<Node> out = component->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = component->GetTerminalNode(INPUT);

        if (out == nullptr || in == nullptr) continue;  // other node is not connected

        std::string out_name = component->GetTerminalNode(OUTPUT)->GetName();
        std::string in_name = component->GetTerminalNode(INPUT)->GetName();

        auto found = parallel_components_map.find(std::make_pair(out_name, in_name));

        if ( found == parallel_components_map.end() ) {
            auto found2 = parallel_components_map.find(std::make_pair(in_name, out_name));

            if ( found == parallel_components_map.end() ) {
                parallel_components_map[std::make_pair(out_name, in_name)] = {component};
            } else {
                (*found2).second.push_back(component);
            }
        } else {
            (*found).second.push_back(component);
        }
    }

    for ( auto const& obj : parallel_components_map ) {
        cd total_admittance = cd(0,0);
        bool short_circuit = false;

       //calculate total admittance
        for ( auto const& component : obj.second ) {
            ComponentType type = component->GetType();

            switch ( type ) {
                case RESISTOR:
                    total_admittance += cd(1 / component->GetValue(), 0);  
                    // Y = 1 / Z = 1 / R
                    break;
                case CAPACITOR:
                    if (omega != 0) {
                        total_admittance += cd(0, (component->GetValue() * omega));  
                        // Y = 1 / Z = j*w*C
                    }
                    break;
                case INDUCTOR:
                    if (omega != 0) {
                        total_admittance += cd(0, (- 1 / (component->GetValue() * omega)));  
                        // Y = 1 / Z = -j / (w*L) 
                        break;
                    } else {
                        short_circuit = true;
                        break;
                    }
                default:
                    break;
            }
        }

        //
        for ( auto const& component : obj.second ) {
            ComponentType type = component->GetType();
            std::string name = component->GetName();

            cd out_value = cd(0,0); 
            cd in_value = cd(0,0); 

            if (component->GetTerminalNode(OUTPUT)->GetType() != GROUND) {
                out_value = node_voltages_[component->GetTerminalNode(OUTPUT)->GetName()];
            }
            if (component->GetTerminalNode(INPUT)->GetType() != GROUND) {
                in_value = node_voltages_[component->GetTerminalNode(INPUT)->GetName()];
            }
            cd V_difference = in_value - out_value;

            if ( omega == 0 ) {
                // if short circuit then other than inductor I = 0   
                if ( short_circuit ) {
                    switch ( type ) {
                        case RESISTOR:
                        case CAPACITOR:
                            passive_component_currents_[name] = cd(0,0);
                            break;
                        case INDUCTOR:
                            // DC inductor current is handled in mna
                            break;
                        default:
                            break;
                    }
                } else {
                    switch ( type ) {
                        case RESISTOR:
                            passive_component_currents_[name] = ( cd(1 / component->GetValue(), 0) / (total_admittance) ) * V_difference * total_admittance;
                            break;
                        case CAPACITOR:
                            passive_component_currents_[name] = cd(0,0);
                            break;
                        default:
                            break;
                    }
                }
            } else {
                // shorts currently only in DC 
                switch ( type ) {
                    case RESISTOR:
                        passive_component_currents_[name] = ( cd(1 / component->GetValue(), 0) / (total_admittance)) * V_difference * total_admittance; 
                        break;
                    case CAPACITOR:
                        passive_component_currents_[name] = ( cd(0, (component->GetValue() * omega)) / (total_admittance)) * V_difference * total_admittance;
                        break;
                    case INDUCTOR:
                        passive_component_currents_[name] = ( cd(0, - 1 / (component->GetValue() * omega)) / (total_admittance)) * V_difference * total_admittance;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

std::ostream& MNAsolver::resultListed(std::ostream &out) {
    out << " Result: " << std::endl;

    for(auto const& pair: node_voltages_){
        out << pair.first 
        << ": "
        << pair.second
        << " V"
        << std::endl;
    }

    for(auto const& pair: voltage_source_currents_){
        out << pair.first 
        << ": "
        << pair.second
        << " A"
        << std::endl;
    }
    
    for(auto const& pair: passive_component_currents_){
        out << pair.first 
        << ": "
        << pair.second
        << " A"
        << std::endl;
    }

    out << std::endl;
    return out;
}