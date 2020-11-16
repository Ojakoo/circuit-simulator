#include <iostream>
#include <complex>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
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

    sf::View view(sf::FloatRect(0, 0, 640.f, 480.f));

    GUIResistor R1("R1");
    R1.setPosition(50, 50);

    GUICapacitor C1("C1");
    C1.setPosition(100, 100);

    GUIInductor L1("L1");
    L1.setPosition(200, 200);

    std::vector<GUIComponent> components;

    components.push_back(R1);
    components.push_back(C1);
    components.push_back(L1);

    sf::Clock deltaClock;

    sf::Vector2f oldPos;
    bool moving = false;
    float zoom = 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            switch ( event.type )
            {
                case sf::Event::Closed:
                    window.close();
                    break;
            
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.zoom(zoom);
                    window.setView(view);
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        moving = true;
                        oldPos = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
                        );
                    }
                    break;

                case  sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        moving = false;
                    }
                    break;

                case sf::Event::MouseMoved:
                    {
                        if (!moving) break;
                        
                        const sf::Vector2f newPos = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                        );

                        const sf::Vector2f deltaPos = oldPos - newPos;

                        view.setCenter(view.getCenter() + deltaPos);
                        window.setView(view);

                        oldPos = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                        );
                        break;
                    }
                case sf::Event::MouseWheelScrolled:
                    if (moving) break;

                    if (event.mouseWheelScroll.delta <= -1) {
                        // Max zoom is 2.0
                        zoom = std::min(2.f, zoom + .1f);
                    } else if (event.mouseWheelScroll.delta >= 1) {
                        // Min zoom is 0.5
                        zoom = std::max(.5f, zoom - .1f);
                    }

                    view.setSize(window.getDefaultView().getSize());
                    view.zoom(zoom);
                    window.setView(view);
                    break;

                default:
                    break; 
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color(148, 143, 129));
        
        // draw components
        for ( auto it : components ) {
            window.draw(it);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;

}