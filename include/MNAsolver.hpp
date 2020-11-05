#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;

class MNAsolver {
    public:
        MNAsolver();

        const VectorXcf& solveSteady(const MatrixXcf& A, const VectorXcf& z) const;

    private:
        MatrixXcf test = MatrixXcf::Random(3,3);

};