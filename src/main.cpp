#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

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


int main ( void ) {

    //MNA-solver test
    MNAsolver MNA = MNAsolver();
    const Eigen::MatrixXcf testA = Eigen::MatrixXcf::Random(3,3);
    const Eigen::VectorXcf testZ = Eigen::VectorXcf::Random(3);
    const Eigen::VectorXcf testX = MNA.solveSteady(testA,testZ);
    std::cout << "A matrix is: \n" << testA << std::endl;
    std::cout << "Z vector is: \n" << testZ << std::endl;
    std::cout << "X vector is: \n" << testX << std::endl;

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

    return 0;

}