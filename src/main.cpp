#include <iostream>
#include <complex>
<<<<<<< HEAD

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
=======
>>>>>>> b8c7c75f56c1420edcabcc6bd0df030f0f46eb1e

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
<<<<<<< HEAD
=======

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
    
>>>>>>> b8c7c75f56c1420edcabcc6bd0df030f0f46eb1e

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
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    */
    return 0;

}