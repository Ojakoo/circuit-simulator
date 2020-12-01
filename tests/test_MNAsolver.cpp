#include <string>
#include <complex>
#include <iostream>
#include <map>

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

        std::map<std::string, int> node_indexes_;
        node_indexes_[ "N001" ] = 0;
        node_indexes_[ "N002" ] = 1;
        node_indexes_[ "N003" ] = 2;
        std::map<std::string, int> voltage_source_indexes_;
        voltage_source_indexes_[ "S1" ] = 3;

        WHEN("") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, node_indexes_, voltage_source_indexes_);

            const VectorXcf x = solver.GetxVector();

            THEN("") {
                CHECK(x.isApprox(Refx));
            }
        }
    }
}

SCENARIO("MNA solver calculates correctly") {
    GIVEN("Circuit to construct matrix from") {

        Circuit c = Circuit();
        
        std::shared_ptr<Node> n1 = c.AddNode("N001");
        std::shared_ptr<Node> n2 = c.AddNode("N002");
        std::shared_ptr<Node> n3 = c.AddNode("N003");
        std::shared_ptr<Node> g = c.AddNode("0");

        std::shared_ptr<DCVoltageSource> s1 = std::make_shared<DCVoltageSource>("S1", 6, g, n1);
        std::shared_ptr<DCVoltageSource> s2 = std::make_shared<DCVoltageSource>("S2", 4, n2, n3);
        std::shared_ptr<DCCurrentSource> s3 = std::make_shared<DCCurrentSource>("S3", 5, n1, n2);

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R2", 0.5, n2, g);
        std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R1", 0.5, n1, n2);
        std::shared_ptr<Resistor> r3 = std::make_shared<Resistor>("R3", 0.5, n3, g);

        c.AddComponent(r1);
        c.AddComponent(r2);
        c.AddComponent(r3);
        c.AddComponent(s1);
        c.AddComponent(s2);
        c.AddComponent(s3);

        c.ConstructMatrices();

        MatrixXcf A = c.GetAMatrix();
        VectorXf z = c.GetZMatrix();
        std::map<std::string, int> node_indexes = c.GetNodeIndexes();
        std::map<std::string, int> voltage_source_indexes = c.GetVoltageSourceIndexes();

        std::map<std::string, cd> node_voltages_ref;
        node_voltages_ref[ "N001" ] = cd(6,0);
        node_voltages_ref[ "N002" ] = cd(1.5,0);
        node_voltages_ref[ "N003" ] = cd(5.5,0);
        std::map<std::string, cd> voltage_source_currents_ref;
        voltage_source_currents_ref[ "S1" ] = cd(-14,0);
        voltage_source_currents_ref[ "S2" ] = cd(-11,0);

        Eigen::VectorXcf Refx = MatrixXf::Zero(5, 1);

        Refx << cd(6,0), cd(1.5,0), cd(5.5,0), cd(-14,0), cd(-11,0);

        WHEN("") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, node_indexes, voltage_source_indexes);

            THEN("") {
                CHECK(solver.GetxVector().isApprox(Refx));
            }

            THEN("solved and reference is same len") {
                CHECK(node_voltages_ref.size() == solver.GetNodeVoltages().size());
            }

            THEN("solved and reference is same len") {
                CHECK(voltage_source_currents_ref.size() == solver.GetVoltageSourceCurrents().size());
            }
        }
    }
}