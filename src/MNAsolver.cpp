#include <map>

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