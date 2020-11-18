#include <iostream>
#include <complex>
#include <math.h>
#include <memory.h>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Eigen/Dense"

// Include components
#include "component.hpp"
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

#define GRID_SIZE 40

typedef std::complex<float> cd;


enum GUIAction {
    NO_ACTION,
    MOVING_COMPONENT,
    ROTATING_COMPONENT,
    DELETING_COMPONENT,
    DRAWING_WIRE
};


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
    bool movingView = false;
    GUIAction action = NO_ACTION;
    GUIComponent *movingComponent = nullptr;
    float zoom = 1;

    sf::VertexArray lines(sf::Lines, 60);

    int k = 0;

    // vertical helper lines
    for ( int i = 0; i <= 640; i+=GRID_SIZE, k+=2) {
        lines[k].position = sf::Vector2f(i, 0);
        lines[k].color = sf::Color(197, 206, 219);
        lines[k + 1].position = sf::Vector2f(i, 480);
        lines[k + 1].color = sf::Color(197, 206, 219);
    }

    //  horizontal helper lines
    for ( int j = 0; j <= 480; j+=GRID_SIZE, k+=2 ) {
        lines[k].position = sf::Vector2f(0, j);
        lines[k].color = sf::Color(197, 206, 219);
        lines[k + 1].position = sf::Vector2f(640, j);
        lines[k + 1].color = sf::Color(197, 206, 219);
    }

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

                        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        bool clicked_component = false;
                        for ( auto it = components.begin(); it != components.end(); it++ ) {
                            if (it->getGlobalBounds().contains(mouse)) {
                                switch ( action ) {
                                    case MOVING_COMPONENT:
                                        if (movingComponent) {
                                            // already moving component
                                            movingComponent = nullptr;
                                        } else {
                                            movingComponent = &(*it);
                                        }
                                        break;
                                    case ROTATING_COMPONENT:
                                        it->rotate(90);
                                        break;
                                    case DELETING_COMPONENT:
                                        components.erase(it);
                                        break;
                                    default:
                                        break;
                                }
                                clicked_component = true;
                                break;
                            }
                        }
                        if (movingComponent && !clicked_component) {
                            // moving a component but mouse is not inside a sprite
                            movingComponent = nullptr;
                        }
                        if (!clicked_component) {
                            movingView = true;
                            oldPos = mouse;
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        // cancel action
                        action = NO_ACTION;
                        movingComponent = nullptr;
                    }
                    break;

                case  sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        movingView = false;
                    }
                    break;

                case sf::Event::MouseMoved:
                    {
                        const sf::Vector2f newPos = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                        );

                        if (!movingView) {
                            if (movingComponent) {
                                movingComponent->setPosition(newPos);
                            }
                            break;
                        };

                        const sf::Vector2f deltaPos = oldPos - newPos;

                        view.setCenter(view.getCenter() + deltaPos);
                        window.setView(view);

                        oldPos = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                        );
                        break;
                    }
                case sf::Event::MouseWheelScrolled:
                    if (movingView) break;

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

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "CTRL+O")) {}
                if (ImGui::MenuItem("Save", "CTRL+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Close")) {
                    window.close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::BeginMenu("Add component.."))
                {
                    if (ImGui::MenuItem("Resistor", "CTRL+R")) {}
                    if (ImGui::MenuItem("Capacitor", "CTRL+C")) {}
                    if (ImGui::MenuItem("Inductor", "CTRL+I")) {}
                    if (ImGui::MenuItem("Voltage source", "CTRL+V")) {}
                    if (ImGui::MenuItem("Current source", "CTRL+J")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Wire", "CTRL+W")) {
                    action = DRAWING_WIRE;
                }
                if (ImGui::MenuItem("Rotate", "CTRL+R")) {
                    action = ROTATING_COMPONENT;
                }
                if (ImGui::MenuItem("Move", "CTRL+M")) {
                    action = MOVING_COMPONENT;
                }
                if (ImGui::MenuItem("Delete", "CTRL+D")) {
                    action = DELETING_COMPONENT;
                }
                //ImGui::Separator();
                //if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                //if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                //if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Simulate"))
            {
                if (ImGui::MenuItem("Steady state analysis")) {}
                if (ImGui::MenuItem("Transient analysis")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        window.clear(sf::Color(148, 143, 129));
        
        // draw components
        for ( auto it : components ) {
            window.draw(it);
        }

        window.draw(lines);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;

}