#include <iostream>
#include <complex>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Eigen/Dense"

// Include components
#include "circuit.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "MNAsolver.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "save_and_load.hpp"
#include "save_and_load.hpp"

// Include GUI components
#include "gui_components/gui_resistor.hpp"
#include "gui_components/gui_capacitor.hpp"
#include "gui_components/gui_inductor.hpp"

typedef std::complex<float> cd;


int main ( void ) {

    sf::RenderWindow window(sf::VideoMode(640, 480), "Circuit Simulator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::View SimView(sf::FloatRect(0, 0, 640.f, 480.f));

    window.setView(SimView);

    GUIResistor R1("R1");
    R1.setPosition(50, 50);

    GUICapacitor C1("C1");
    C1.setPosition(100, 100);

    GUIInductor L1("L1");
    L1.setPosition(200, 200);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            switch ( event.type )
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;
            
                // catch the resize events
                case sf::Event::Resized:
                    // update the view to the new size of the window
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    break;
                
                // mouse pressed
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // transform the mouse position from window coordinates to world coordinates
                        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        // retrieve the bounding box of the sprite
                        sf::FloatRect bounds = R1.getGlobalBounds();

                        // hit test
                        if (bounds.contains(mouse))
                        {
                            std::cout << R1.GetName() << std::endl;
                        }
                    }
                    break;

                default:
                    break; 
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color(148, 143, 129));
        window.draw(R1);
        window.draw(C1);
        window.draw(L1);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;

}