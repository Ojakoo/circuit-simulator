#include "MNAsolver.hpp"

MNAsolver::MNAsolver(){}

const VectorXcf MNAsolver::solveSteady(const MatrixXcf& A, const VectorXcf& z) const{
    return A.inverse()*z;
}

void MNAsolver::setCurrents(const std::map<std::string,float>& node_voltages_, const std::list<shared_ptr<Componennt>>& components) const{
    
    
    for ( auto const& component : components_ ){
        std::shared_ptr<Node> out = component->GetTerminalNode(OUTPUT);
        std::shared_ptr<Node> in = component->GetTerminalNode(INPUT);


        ComponentType type = component->GetType();
        ComponentClass cls = component->GetClass();

        std::string out_name = out->GetName();
        std::string in_name = in->GetName();

        float out_value = node_voltage_[out_name];
        float in_value = node_voltage_[in_name];

        float V_difference = in_value - out_value;


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
                        if (omega)
                            admittance = std::complex<float>(
                                0, component->GetValue() * omega
                            );  // Y = 1 / Z = j*w*C
                        break;
                    case INDUCTOR:
                        if (omega)
                            admittance = std::complex<float>(
                                0, 1 / (component->GetValue() * omega)
                            );  // Y = 1 / Z = 1 / (j*w*L)
                        break;
                    default:
                        break;
                }
                float current = V_difference * admittance

                component->SetCurrent(current) 
                
                
                break;
            case ACTIVE:
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
