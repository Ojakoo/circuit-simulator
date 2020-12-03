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
        void solveSteady(const MatrixXcf& A, const VectorXcf& z, std::map<std::string, int> node_indexes_, std::map<std::string, int> voltage_source_indexes_);
        const VectorXcf GetxVector() const;
        const std::map<std::string, cd> GetNodeVoltages() const;
        const std::map<std::string, cd> GetVoltageSourceCurrents() const;

        void setCurrents(const std::list<std::shared_ptr<Component>> components) const;

    private:
        MatrixXcf test = MatrixXcf::Zero(3,3);
        VectorXcf x_;
        std::map<std::string, cd> node_voltages_;
        std::map<std::string, cd> voltage_source_currents_;
};

std::ostream &operator<<(std::ostream& out, const MNAsolver& solver);