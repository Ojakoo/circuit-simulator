#include <string>
#include <complex>
#include <iostream>

#include "doctest.h"
#include "circuit.hpp"
#include "component.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "voltage_source.hpp"
#include "current_source.hpp"
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

        std::shared_ptr<VoltageSource> s1 = std::make_shared<VoltageSource>("S1", 6, n1, n2);
        std::shared_ptr<CurrentSource> s2 = std::make_shared<CurrentSource>("S2", 2, n3, n1);
        std::shared_ptr<CurrentSource> s3 = std::make_shared<CurrentSource>("S3", 4, g, n3);

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

SCENARIO("Producing matrices") {
    GIVEN("Circuit with multiple sources and resistors") {

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

        Eigen::MatrixXcf RefA = MatrixXcf::Zero(5, 5);

        RefA << cd(2,0), cd(-2,0), cd(0,0), cd(1,0), cd(0,0),
                cd(-2,0), cd(4,0),  cd(0,0), cd(0,0), cd(-1,0),
                cd(0,0), cd(0,0), cd(2,0), cd(0,0), cd(1,0),
                cd(1,0), cd(0,0), cd(0,0), cd(0,0), cd(0,0),
                cd(0,0), cd(-1,0), cd(1,0), cd(0,0), cd(0,0);

        Eigen::VectorXf Refz = MatrixXf::Zero(5, 1);

        Refz << -5, 5, 0, 6, 4;

        WHEN("") {
            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("Matricies are the right size") {
                CHECK(A.rows() == 5);
                CHECK(A.cols() == 5);
                CHECK(z.rows() == 5);
                CHECK(z.cols() == 1);
            }

            THEN("Matrix is built right") {
                CHECK(z.isApprox(Refz));
                CHECK(A.isApprox(RefA));
            }

            std::cout << "z:\n" << z << "\n\n" << "A\n" << A << std::endl;
            //std::cout << "node_indexes_:\n" << c.node_indexes_ << "\n\nvoltage_indexes_:\n" << c.source_indexes_ << std::endl;
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
                CHECK(A.rows() == 4);
                CHECK(A.cols() == 4);
                CHECK(z.rows() == 4);
                CHECK(z.cols() == 1);
            }

            THEN("Matricies are are built correctly") {
                Matrix4cf m;
                m << cd(0.1, 0), cd(-0.1, 0), cd(1.0, 0), cd(0.0, 0.0),
                     cd(-0.1, 0), cd(0.1, 0), cd(0.0, 0), cd(1.0, 0.0),
                     cd(1.0, 0), cd(0.0, 0), cd(0.0, 0), cd(0.0, 0.0),
                     cd(0.0, 0.0), cd(1.0, 0.0), cd(0.0, 0.0), cd(0.0, 0.0);
                Vector4f e(0, 0, 5, 0);
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

        std::shared_ptr<VoltageSource> V1 = std::make_shared<VoltageSource>("S1", 6, g, n1);
        
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

SCENARIO("Circuit with inductor") {
    GIVEN("Circuit to construct matrix from") {

        Circuit c = Circuit();
        
        auto n1 = c.AddNode("N001");
        auto n2 = c.AddNode("N002");
        auto g = c.AddNode("0");

        auto v1 = std::make_shared<VoltageSource>("V1", 10, g, n1);
        auto r1 = std::make_shared<Resistor>("R1", 5, n1, n2);
        auto l1 = std::make_shared<Inductor>("L1", 2, n2, g);

        c.AddComponent(v1);
        c.AddComponent(r1);
        c.AddComponent(l1);

        WHEN("Matrices are constructed") {
            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("A matrix and z vector sizes are correct") {
                CHECK(A.rows() == 4);
                CHECK(A.cols() == 4);
                CHECK(z.rows() == 4);
                CHECK(z.cols() == 1);
            }
    
            AND_THEN("A matrix is built correctly") {
                Matrix4cf m;
                m << cd(0.2, 0), cd(-0.2, 0), cd(1.0, 0), cd(0.0, 0.0),
                     cd(-0.2, 0), cd(0.2, 0), cd(0.0, 0), cd(-1.0, 0.0),
                     cd(1.0, 0), cd(0.0, 0), cd(0.0, 0), cd(0.0, 0.0),
                     cd(0.0, 0.0), cd(-1.0, 0.0), cd(0.0, 0.0), cd(0.0, 0.0);
                CHECK(A.isApprox(m));
            }

            AND_THEN("z vector is built correctly") {
                Vector4f e(0, 0, 10, 0);
                CHECK(z.isApprox(e));
            }
        }
    }
}

SCENARIO("Circuit with inductors in series") {
    GIVEN("Circuit to construct matrix from") {

        Circuit c = Circuit();
        
        auto n1 = c.AddNode("N001");
        auto n2 = c.AddNode("N002");
        auto n3 = c.AddNode("N003");
        auto g = c.AddNode("0");

        auto v1 = std::make_shared<VoltageSource>("V1", 10, g, n1);
        auto r1 = std::make_shared<Resistor>("R1", 5, n1, n2);
        auto l1 = std::make_shared<Inductor>("L1", 2, n2, n3);
        auto l2 = std::make_shared<Inductor>("L2", 10, n3, g);

        c.AddComponent(v1);
        c.AddComponent(r1);
        c.AddComponent(l1);
        c.AddComponent(l2);

        WHEN("Matrices are constructed") {
            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("A matrix and z vector sizes are correct") {
                CHECK(A.rows() == 6);
                CHECK(A.cols() == 6);
                CHECK(z.rows() == 6);
                CHECK(z.cols() == 1);
            }

            AND_THEN("Result is correct") {
                VectorXf e(6, 1);
                e << 10, 0, 0, -2, -2, -2;
                CHECK((A.inverse() * z).isApprox(e));
            }
        }
    }
}


SCENARIO("Circuit with inductors and capacitors") {
    GIVEN("Circuit to construct matrix from") {

        Circuit c = Circuit();

        auto n1 = c.AddNode("N001");
        auto n2 = c.AddNode("N002");
        auto g = c.AddNode("0");

        auto v1 = std::make_shared<VoltageSource>("V1", 10, g, n1);
        auto r1 = std::make_shared<Resistor>("R1", 5, n1, n2);
        auto c1 = std::make_shared<Capacitor>("C1", 2, n2, g);
        auto l1 = std::make_shared<Inductor>("L1", 10, n2, g);
        auto r2 = std::make_shared<Resistor>("R2", 20, n2, g);

        c.AddComponent(v1);
        c.AddComponent(r1);
        c.AddComponent(c1);
        c.AddComponent(l1);
        c.AddComponent(r2);

        WHEN("Matrices are constructed") {
            c.ConstructMatrices();

            MatrixXcf A = c.GetAMatrix();
            VectorXf z = c.GetZMatrix();

            THEN("A matrix and z vector sizes are correct") {
                CHECK(A.rows() == 4);
                CHECK(A.cols() == 4);
                CHECK(z.rows() == 4);
                CHECK(z.cols() == 1);
            }

            AND_THEN("Result is correct") {
                VectorXf e(4, 1);
                e << 10, 0, -2, -2;
                CHECK((A.inverse() * z).isApprox(e));
            }
        }
    }
}