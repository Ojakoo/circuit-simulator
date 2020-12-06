
#include "MNAsolver.hpp"

MNAsolver::MNAsolver(){}

void MNAsolver::solveSteady(const MatrixXcf& A, const VectorXcf& z, std::map<std::string, int> node_indexes, std::map<std::string, int> voltage_source_indexes) {
    x_ = A.inverse()*z;
    node_voltages_.clear();
    voltage_source_currents_.clear();

    for ( auto const& i : node_indexes ) {
        node_voltages_[ i.first ] = x_( i.second );
    }
    for ( auto const& i : voltage_source_indexes ) {
        voltage_source_currents_[ i.first ] = x_( i.second );
    }
}

const VectorXcf MNAsolver::GetxVector() const {
    return x_;
}

const std::map<std::string, cd> MNAsolver::GetNodeVoltages() const {
    return node_voltages_;
}

const std::map<std::string, cd> MNAsolver::GetVoltageSourceCurrents() const {
    return voltage_source_currents_;
}

std::ostream &operator<<(std::ostream& out, const MNAsolver& solver) {
    out << "\nnode voltages";
    for ( auto it : solver.GetNodeVoltages() ) {
        out << "\n" << it.first << " " << it.second;
    }
    out << "\nvoltage source currents";
    for ( auto it : solver.GetVoltageSourceCurrents() ) {
        out << "\n" << it.first << " " << it.second;
    }
    return out.flush();
}


void MNAsolver::setCurrents( const std::list<std::shared_ptr<Component>> components){
    // int omega = 0.0; //for ac circuit
    passive_component_currents_.clear();

    for ( auto const& component : components ){
        std::shared_ptr<Node> out = component->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = component->GetTerminalNode(INPUT);

        ComponentType type = component->GetType();
        ComponentClass cls = component->GetClass();
        std::string comp_name = component->GetName();

        std::string out_name = out->GetName();
        std::string in_name = in->GetName();

        std::complex<float> out_value = node_voltages_[out_name];
        std::complex<float> in_value = node_voltages_[in_name];

        std::complex<float> V_difference = in_value - out_value;


        std::complex<float> admittance;
        std::complex<float> current;

        switch ( cls ) {
            case PASSIVE:
                /*
                Calculating currents only for passive components
                */
                switch ( type ) {
                    case RESISTOR:
                        admittance = std::complex<float>(
                            1 / component->GetValue(), 0
                        );  // Y = 1 / Z = 1 / R
                        current = V_difference * admittance;
                        passive_component_currents_[ comp_name] = current;
                        break;
                    case CAPACITOR:
                    //if we apply ac sources
                     /*   if (omega){
                            admittance = std::complex<float>(
                                0, component->GetValue() * omega
                            );  // Y = 1 / Z = j*w*C
                            current = V_difference * admittance;
                        }
                        */
                        passive_component_currents_[ comp_name] = std::complex<float>(0,0);
                        break;
                        /*
                    case INDUCTOR:
                    //if we apply ac sources
                        if (omega){
                            admittance = std::complex<float>(
                                0, 1 / (component->GetValue() * omega)
                            );  // Y = 1 / Z = 1 / (j*w*L)
                            current = V_difference * admittance;
                        }
                        passive_component_currents_[ comp_name] = std::complex<float>(0,0);
                        break;
                        */
                    default:
                        break;
                }
                 
                
                
                break;
            case ACTIVE: //do nothing
            /*
                switch ( type ) {
                    case DC_VOLTAGE_SOURCE:
                        break;
                    case DC_CURRENT_SOURCE:
                        break;
                    default:
                        break;
                } 
            */
                break;
        }
    }
}