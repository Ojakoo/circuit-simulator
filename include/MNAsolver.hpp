#pragma once

#include <iostream>
#include <map>
#include <list>

#include "component.hpp"
#include "node.hpp"
#include "circuit.hpp"
#include "Eigen/Dense"

using namespace Eigen;

typedef std::complex<float> cd;

class MNAsolver {
    public:
        MNAsolver();
        void solveSteady(
            const MatrixXcf& A, 
            const VectorXcf& z, 
            float omega,
            std::map<std::string, int> node_indexes, 
            std::map<std::string, int> voltage_source_indexes, 
            std::map<std::string, int> inductor_indexes
        );
        const VectorXcf GetxVector() const { return x_; };
        const VectorXcf GetiVector() const { return i_; };
        const std::map<std::string, cd> GetNodeVoltages() const { return node_voltages_; };
        const std::map<std::string, cd> GetVoltageSourceCurrents() const { return voltage_source_currents_; };

        void setCurrents(const std::list<std::shared_ptr<Component>> components, float omega);
        
        std::ostream& resultListed(std::ostream &out);


    private:
        MatrixXcf test = MatrixXcf::Zero(3,3);
        VectorXcf x_;
        VectorXcf i_;
        std::map<std::string, cd> node_voltages_;
        std::map<std::string, cd> voltage_source_currents_;
        std::map<std::string, cd> passive_component_currents_;
};

std::ostream &operator<<(std::ostream& out, const MNAsolver& solver);