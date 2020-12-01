
#include "doctest.h"
#include "MNAsolver.hpp"
#include "circuit.hpp"
#include "component.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "node.hpp"
#include "save_and_load.hpp"
#include "Eigen/Dense"

typedef std::complex<float> cd;

SCENARIO("MNA solver calculates correctly") {
    GIVEN("Ready constructed matrix") {
        Eigen::Matrix4cf A;

        A << cd(2,0),  cd(0,0),  cd(0,0),  cd(-1,0),
                cd(0,0),  cd(4,0),  cd(-2,0), cd(1,0),
                cd(0,0),  cd(-2,0), cd(2,0),  cd(0,0),
                cd(-1,0), cd(1,0),  cd(0,0),  cd(0,0);

        Eigen::Vector4f z;

        z << 2, 0, 2, 6;

        Eigen::Vector4cf Refx;

        Refx << cd(-2,0), cd(4,0), cd(5,0), cd(-6,0);


        WHEN("") {
            MNAsolver solver = MNAsolver();

            VectorXcf x = solver.solveSteady(A, z);

            THEN("") {
                CHECK(x.isApprox(Refx));
            }
        }
    }
}