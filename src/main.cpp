#include <iostream>
#include <complex>

#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "circuit.hpp"
#include "save_and_load.hpp"
#include "MNAsolver.hpp"
#include "save_and_load.hpp"
#include "Eigen/Dense"

typedef std::complex<float> cd;

int main ( void ) {

    //Circuit c = LoadNetList("../../tests/netlists/netlist_good.txt");

    //SaveNetList(c, "l.txt");

    //MNA-solver test
    /*
    MNAsolver MNA = MNAsolver();
    const Eigen::MatrixXcf testA = Eigen::MatrixXcf::Random(3,3);
    const Eigen::VectorXcf testZ = Eigen::VectorXcf::Random(3);
    const Eigen::VectorXcf testX = MNA.solveSteady(testA,testZ);
    std::cout << "A matrix is: \n" << testA << std::endl;
    std::cout << "Z vector is: \n" << testZ << std::endl;
    std::cout << "X vector is: \n" << testX << std::endl;
    */

    //Matrix formation test
    /*
    const Eigen::MatrixXcf A = Eigen::MatrixXcf::Zero(3,3);
    const Eigen::MatrixXf real = Eigen::MatrixXf::Zero(3,3);
    const Eigen::MatrixXcf sum = A + real;
    std::cout << "A matrix is: \n" << A << std::endl;
    std::cout << "real matrix is: \n" << real << std::endl;
    std::cout << "sum matrix is: \n" << sum << std::endl;
    std::cout << A.size() << std::endl;
    std::cout << A.rows() << std::endl;
    std::cout << A.cols() << std::endl;
    */

    //Test for sMarix output

    Eigen::Matrix4cf Ref;

    Ref << cd(2,0), cd(0,0), cd(0,0), cd(0,0),
    cd(0,0), cd(4,0), cd(-2,0), cd(0,0),
    cd(0,0), cd(-2,0), cd(2,0), cd(0,0),
    cd(0,0), cd(0,0), cd(0,0), cd(0,0);

    std::cout << Ref << std::endl;
    
    Circuit c = Circuit();

    std::shared_ptr<Node> n1 = c.AddNode("N001");
    std::shared_ptr<Node> n2 = c.AddNode("N002");
    std::shared_ptr<Node> g = c.AddNode("0");

    std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 50, n1, n2);
    std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 50, n2, g);
    std::shared_ptr<Capacitor> c1 = std::make_shared<Capacitor>("C1", 1, n2, g);
    std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 2, n2, g);
    std::shared_ptr<DCVoltageSource> s1 = std::make_shared<DCVoltageSource>("S1", 10, n1, g);

    c.AddComponent(r1);
    c.AddComponent(r2);
    c.AddComponent(c1);
    c.AddComponent(l1);
    c.AddComponent(s1);

    Eigen::MatrixXcf A = c.sMatrix();

    std::cout << A << std::endl;
    

    /*
    std::shared_ptr<Node> N001 = std::make_shared<Node>("N001");
    std::shared_ptr<Node> N002 = std::make_shared<Node>("N002");

    N002->SetNodeType(GROUND);

    Resistor R1 = Resistor("R1", 50);
    Inductor L1 = Inductor("L1", 0.001);
    Capacitor C1 = Capacitor("C1", 0.0009);
    DCVoltageSource V1 = DCVoltageSource("V1", 10);
    DCCurrentSource J1 = DCCurrentSource("J1", 2);

    R1.ConnectNodeToTerminal(N001, OUTPUT);
    R1.ConnectNodeToTerminal(N002, INPUT);

    L1.ConnectNodeToTerminal(N002, INPUT);

    std::cout << R1 << std::endl;
    std::cout << L1 << std::endl;
    std::cout << C1 << std::endl;
    std::cout << V1 << std::endl;
    std::cout << J1 << std::endl;
    */
    return 0;
}