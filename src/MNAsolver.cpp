#include "MNAsolver.hpp"

MNAsolver::MNAsolver(){}

const VectorXcf& MNAsolver::solveSteady(const MatrixXcf& A, const VectorXcf& z) const{
    const VectorXcf& x = A.inverse()*z;
    return x;
}