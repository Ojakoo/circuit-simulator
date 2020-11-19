#include <string>
#include <complex>

#include "doctest.h"
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

SCENARIO("Constructing circuit") {
    GIVEN("An empty circuit") {

        Circuit c = Circuit();

        WHEN("Two components are added") {

            std::shared_ptr<Node> n1 = c.AddNode("N001");
            std::shared_ptr<Node> n2 = c.AddNode("N002");
            std::shared_ptr<Node> n3 = c.AddNode("N003");

            std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 50, n1, n2);
            std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 50, n2, n3);

            c.AddComponent(r1);
            c.AddComponent(r2);

            THEN("There is 2 components in circuit") {
                CHECK(c.GetComponents().size() == 2);
            }
        }
    }
}

SCENARIO("Producing A and z matrix from circuit with resistors") {
    GIVEN("A circuit with resistors") {
        Circuit c = Circuit();

        std::shared_ptr<Node> n1 = c.AddNode("N001");
        std::shared_ptr<Node> n2 = c.AddNode("N002");
        std::shared_ptr<Node> n3 = c.AddNode("N003");
        std::shared_ptr<Node> g = c.AddNode("0");

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 0.5, n1, g);
        std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 0.5, n2, g);
        std::shared_ptr<Resistor> r3 = std::make_shared<Resistor>("R3", 0.5, n2, n3);

        std::shared_ptr<DCVoltageSource> s1 = std::make_shared<DCVoltageSource>("S1", 6, n1, n2);
        std::shared_ptr<DCCurrentSource> s2 = std::make_shared<DCCurrentSource>("S2", 2, n3, n1);
        std::shared_ptr<DCCurrentSource> s3 = std::make_shared<DCCurrentSource>("S3", 4, g, n3);

        c.AddComponent(r1);
        c.AddComponent(r2);
        c.AddComponent(r3);
        c.AddComponent(s1);
        c.AddComponent(s2);
        c.AddComponent(s3);

        Eigen::Matrix4cf RefA;

        RefA << cd(2,0),  cd(0,0),  cd(0,0),  cd(-1,0),
                cd(0,0),  cd(4,0),  cd(-2,0), cd(1,0),
                cd(0,0),  cd(-2,0), cd(2,0),  cd(0,0),
                cd(-1,0), cd(1,0),  cd(0,0),  cd(0,0);

        Eigen::Vector4f Refz;

        Refz << 2, 0, 2, 6;

        WHEN("Matricies are constructed") {

            THEN("There is 6 components in circuit") {
                CHECK(c.GetComponents().size() == 6);
            }

            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("Matricies are the right size") {
                CHECK(A.rows() == 4);
                CHECK(A.cols() == 4);
                CHECK(z.rows() == 4);
                CHECK(z.cols() == 1);
            }

            THEN("Matrix is built right") {
                CHECK(z.isApprox(Refz));
                CHECK(A.isApprox(RefA));
            }
        }
    }
}

SCENARIO("Producing matricies from circuit that is read from file") {
    GIVEN("a good file to read") {
        const std::string fname = "../../tests/netlists/netlist_good.txt";

        WHEN("the the file  is read") {

            Circuit c = LoadNetList(fname);

            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();
    
            THEN("There is 4 components in circuit") {
                CHECK(c.GetComponents().size() == 4);
            }

            THEN("Matricies are the right size") {
                CHECK(A.rows() == 3);
                CHECK(A.cols() == 3);
                CHECK(z.rows() == 3);
                CHECK(z.cols() == 1);
            }

            THEN("Matricies are are built correctly") {
                Matrix3cf m;
                m << cd(0.1, 0), cd(-0.1, 0), cd(1.0, 0),
                     cd(-0.1, 0), cd(0.1, 0), cd(0.0, 0),
                     cd(1.0, 0), cd(0.0, 0), cd(0.0, 0);
                Vector3f e(0, 0, 5);
                CHECK(z.isApprox(e));
                CHECK(A.isApprox(m));
            }
        }
    }
}

SCENARIO("Testing matrix construction when component is not connected") {
    GIVEN("A circuit with a voltage source and unconnected resistor") {
        Circuit c = Circuit();

        std::shared_ptr<Node> n1 = c.AddNode("N001");
        std::shared_ptr<Node> g = c.AddNode("0");

        std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 0.5);
        r1->ConnectNodeToTerminal(n1, INPUT);

        std::shared_ptr<DCVoltageSource> V1 = std::make_shared<DCVoltageSource>("S1", 6, g, n1);
        
        c.AddComponent(r1);
        c.AddComponent(V1);

        WHEN("Matricies are constructed") {

            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("There is 2 components in circuit") {
                CHECK(c.GetComponents().size() == 2);
            }

            THEN("Matricies are the right size") {
                CHECK(A.rows() == 2);
                CHECK(A.cols() == 2);
                CHECK(z.rows() == 2);
                CHECK(z.cols() == 1);
            }

            THEN("Matricies are are built correctly") {
                Matrix2cf m;
                m << cd(0, 0), cd(1, 0),
                     cd(1, 0),   cd(0 , 0);
                Vector2f e(0, 6);
                CHECK(z.isApprox(e));
                CHECK(A.isApprox(m));
            }
        }
    }
}