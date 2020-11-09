#include "MNAsolver.hpp"

MNAsolver::MNAsolver(){}

const VectorXcf MNAsolver::solveSteady(const MatrixXcf& A, const VectorXcf& z) const{
    return A.inverse()*z;
}