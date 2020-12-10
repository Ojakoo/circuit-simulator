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
#include "voltage_source.hpp"
#include "current_source.hpp"
#include "node.hpp"
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
        std::map<std::string, int> inductor_indexes_;

        WHEN("") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, 0, node_indexes_, voltage_source_indexes_, inductor_indexes_);

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

        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 6, g, n1);
        std::shared_ptr<VoltageSource> s2 = std::make_shared<VoltageSource>("S2", 4, n2, n3);
        std::shared_ptr<CurrentSource> s3 = std::make_shared<CurrentSource>("S3", 5, n1, n2);

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
        std::map<std::string, int> inductor_indexes = c.GetInductorIndexes();

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

            solver.solveSteady(A, z, c.GetOmega(), node_indexes, voltage_source_indexes, inductor_indexes);

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

SCENARIO("Producing matrices from circuit with reactive elements") {
    GIVEN("Circuit with omega") {

        Circuit c = Circuit();

        std::shared_ptr<Node> n1 = c.AddNode("N001");
        std::shared_ptr<Node> n2 = c.AddNode("N002");
        std::shared_ptr<Node> n3 = c.AddNode("N003");
        std::shared_ptr<Node> g = c.AddNode("0");

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 6, g, n2);
        std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 0.002, n1, g);
        std::shared_ptr<Capacitor> c1 = std::make_shared<Capacitor>("C1", 0.00025, n2, n3);
        std::shared_ptr<Capacitor> c2 = std::make_shared<Capacitor>("C1", 0.0003333, n1, n2);

        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 4, g, n3);
        c.SetOmega( 2000 );

        c.AddComponent(r1);
        c.AddComponent(l1);
        c.AddComponent(c1);
        c.AddComponent(c2);
        c.AddComponent(s1);

        c.ConstructMatrices();

        MatrixXcf A = c.GetAMatrix();
        VectorXf z = c.GetZMatrix();
        std::map<std::string, int> node_indexes = c.GetNodeIndexes();
        std::map<std::string, int> voltage_source_indexes = c.GetVoltageSourceIndexes();
        std::map<std::string, int> inductor_indexes = c.GetInductorIndexes();

        WHEN("") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, c.GetOmega(), node_indexes, voltage_source_indexes, inductor_indexes);
        }
    }

    GIVEN("Another circuit with Omega defined") {

        Circuit c = Circuit();

        std::shared_ptr<Node> g = c.AddNode("0");
        std::shared_ptr<Node> n1 = c.AddNode("N1");
        std::shared_ptr<Node> n2 = c.AddNode("N2");
        std::shared_ptr<Node> n3 = c.AddNode("N3");

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 100, n1, n2);
        std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 0.005, n2, n3);
        std::shared_ptr<Capacitor> c1 = std::make_shared<Capacitor>("C1", 0.00002, n3, g);
        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 12, g, n1);
        c.SetOmega( 314.159265359 );
        
        c.AddComponent(r1);
        c.AddComponent(l1);
        c.AddComponent(c1);
        c.AddComponent(s1);

        c.ConstructMatrices();

        MatrixXcf A = c.GetAMatrix();
        VectorXf z = c.GetZMatrix();
        std::map<std::string, int> node_indexes = c.GetNodeIndexes();
        std::map<std::string, int> voltage_source_indexes = c.GetVoltageSourceIndexes();
        std::map<std::string, int> inductor_indexes = c.GetInductorIndexes();

        WHEN("solved") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, c.GetOmega(), node_indexes, voltage_source_indexes, inductor_indexes);

            Eigen::VectorXcf Refx = MatrixXf::Zero(4, 1);
            Refx << cd(12,0), cd(8.55495,-5.42883), cd(8.64023,-5.48295), cd(-0.0344505,-0.0542883);
            
            CHECK(solver.GetxVector().isApprox(Refx));
            
            THEN("Check that currents are correctly calculated") {
                solver.setCurrents( c.GetComponents(), c.GetOmega() );
            }
        }
    }

    GIVEN("Dc circuit with a inductor and resistor in series parellel") {
        std::cout << "current test start\n";

        Circuit c = Circuit();
        
        std::shared_ptr<Node> g = c.AddNode("0");
        std::shared_ptr<Node> n1 = c.AddNode("N1");
        std::shared_ptr<Node> n2 = c.AddNode("N2");
        std::shared_ptr<Node> n3 = c.AddNode("N3");

        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 5, g, n1);
        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 10, n1, n2);
        std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 10, n2, n3);
        std::shared_ptr<Resistor> r3 = std::make_shared<Resistor>("R3", 5, n3, g);       
        std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 0.001, n1, n3);
        
        c.AddComponent(r1);
        c.AddComponent(r2);
        c.AddComponent(r3);
        c.AddComponent(l1);
        c.AddComponent(s1);

        c.ConstructMatrices();

        MatrixXcf A = c.GetAMatrix();
        VectorXf z = c.GetZMatrix();
        std::map<std::string, int> node_indexes = c.GetNodeIndexes();
        std::map<std::string, int> voltage_source_indexes = c.GetVoltageSourceIndexes();
        std::map<std::string, int> inductor_indexes = c.GetInductorIndexes();

        WHEN("solved") {
            MNAsolver solver = MNAsolver();

            std::cout << "A:\n" << A << std::endl;
            std::cout << "z:\n" << z << std::endl;

            solver.solveSteady(A, z, c.GetOmega(), node_indexes, voltage_source_indexes, inductor_indexes);

            Eigen::VectorXcf Refx = MatrixXf::Zero(5, 1);
            Refx << cd(5,0), cd(5,0), cd(5, 0), cd(1, 0), cd(1, 0);
            
            //CHECK(solver.GetxVector().isApprox(Refx));
            
            THEN("Check that currents are correctly calculated") {
                solver.setCurrents( c.GetComponents(), c.GetOmega() );

                solver.resultListed( std::cout );
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

        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 4, g, n1);

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 0.5, n1, n2);
        std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 0.5, n2, n3);
        std::shared_ptr<Resistor> r3 = std::make_shared<Resistor>("R3", 0.5, n3, g);
        std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 5, n2, n3);

        c.AddComponent(r1);
        c.AddComponent(r2);
        c.AddComponent(r3);
        c.AddComponent(s1);
        c.AddComponent(l1);

        c.ConstructMatrices();

        MatrixXcf A = c.GetAMatrix();
        VectorXf z = c.GetZMatrix();
        std::map<std::string, int> node_indexes = c.GetNodeIndexes();
        std::map<std::string, int> voltage_source_indexes = c.GetVoltageSourceIndexes();
        std::map<std::string, int> inductor_indexes = c.GetInductorIndexes();

        Eigen::VectorXcf Refx = MatrixXf::Zero(5, 1);
        Refx << cd(4,0), cd(2,0), cd(2,0), cd(4,0), cd(-4,0);
        
        WHEN("") {
            MNAsolver solver = MNAsolver();

            solver.solveSteady(A, z, 0, node_indexes, voltage_source_indexes, inductor_indexes);

            CHECK(solver.GetxVector().isApprox(Refx));

            THEN("Check that currents are correctly calculated") {
                solver.setCurrents( c.GetComponents(), 0 );
            }
        }
    }
}

