#pragma once

#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;

class MNAsolver {
    public:
        MNAsolver();

        const VectorXcf solveSteady(const MatrixXcf& A, const VectorXcf& z) const;

        void setCurrents(const std::map<std::string,int>& node_voltages_, const std::list<shared_ptr<Componennt>>& components) const;

    private:
        MatrixXcf test = MatrixXcf::Zero(3,3);

};