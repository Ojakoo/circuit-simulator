#include <iostream>
#include <complex>
#include <memory.h>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Eigen/Dense"

// Include components
#include "MNAsolver.hpp"
#include "save_and_load.hpp"
#include "save_and_load.hpp"

// Include GUI components
#include "gui_components/gui_resistor.hpp"
#include "gui_components/gui_capacitor.hpp"
#include "gui_components/gui_inductor.hpp"
#include "gui_components/gui_voltage_source.hpp"

#include "circuit.hpp"

#define GRID_SIZE 40

typedef std::complex<float> cd;


enum GUIAction {
    NO_ACTION,
    MOVING_COMPONENT,
    ROTATING_COMPONENT,
    DELETING_COMPONENT,
    DRAWING_WIRE,
    ADDING_COMPONENT
};

class TestWindow : public sf::RenderWindow {
    public:
        TestWindow(int width, int height, const std::string &title)
            : sf::RenderWindow(sf::VideoMode(width, height), title) { }
};


int main ( void ) {

    TestWindow window(640, 480, "Circuit Simulator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    Circuit circuit;

    int resistors = 0;
    int inductors = 0;
    int capacitors = 0;
    int sources = 0;

    sf::View view(sf::FloatRect(0, 0, 640.f, 480.f));

    std::list<std::shared_ptr<GUIComponent>> components;

    sf::Clock deltaClock;

    sf::Vector2f oldPos;
    bool movingView = false;  // is user moving/dragging view?
    GUIAction action = NO_ACTION;  // current action performed by user
    std::shared_ptr<GUIComponent> movingComponent= nullptr;  // pointer to component being moved
    GUIComponent *addingComponent = nullptr;  // pointer to component being added
    float zoom = 1;  // current zoom of view

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

    std::shared_ptr<GUIInductor>  L1= std::make_shared<GUIInductor>("L1");
    circuit.AddComponent(L1->GetComponent());
    components.push_back(L1);

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

                        sf::Vector2f mouse = window.mapPixelToCoords(
                            sf::Mouse::getPosition(window)
                        );

                        bool clicked_component = false;
                        for (auto it = components.begin(); it != components.end(); it++) {
                            if ((*it)->getGlobalBounds().contains(mouse)) {
                                switch ( action ) {
                                    case MOVING_COMPONENT:
                                        if (movingComponent) {
                                            // already moving component
                                            movingComponent = nullptr;
                                        } else {
                                            movingComponent = *it;
                                        }
                                        break;
                                    case ROTATING_COMPONENT:
                                        (*it)->rotate(90);
                                        break;
                                    case DELETING_COMPONENT:
                                        circuit.RemoveComponent((*it)->GetComponent());
                                        components.erase(it);
                                        break;
                                    default:
                                        break;
                                }
                                clicked_component = true;
                                break;
                            }
                        }
                        
                        if (addingComponent) {
                            switch ( addingComponent->GetType() ) {
                                case RESISTOR:
                                    resistors++;
                                    break;
                                case CAPACITOR:
                                    capacitors++;
                                    break;
                                case INDUCTOR:
                                    inductors++;
                                    break;
                                default:
                                    break;
                            }
                            circuit.AddComponent(addingComponent->GetComponent());
                            addingComponent = nullptr;
                            action = NO_ACTION;
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
                        // cancel all actions
                        action = NO_ACTION;
                        movingComponent = nullptr;
                        if (addingComponent) {
                            components.pop_back();
                        }
                        addingComponent = nullptr;
                    }
                    break;

                case sf::Event::MouseButtonReleased:
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
                            if (movingComponent)
                                movingComponent->setPosition(newPos);
                            
                            if (addingComponent) {
                                addingComponent->setPosition(newPos);
                            }
                            break;
                        }

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
                if (ImGui::MenuItem("Open", "CTRL+O", false, false)) {}
                if (ImGui::MenuItem("Save", "CTRL+S", false, false)) {}
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
                    if (ImGui::MenuItem("Resistor", "CTRL+R")) {
                        components.push_back(
                            std::make_shared<GUIResistor>("R" + std::to_string(resistors))
                        );
                        addingComponent = &(*components.back());
                        action = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Capacitor", "CTRL+C")) {
                        components.push_back(
                            std::make_shared<GUICapacitor>("C" + std::to_string(capacitors))
                        );
                        addingComponent = &(*components.back());
                        action = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Inductor", "CTRL+I")) {
                        components.push_back(
                            std::make_shared<GUIInductor>("L" + std::to_string(inductors))
                        );
                        addingComponent = &(*components.back());
                        action = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Voltage source", "CTRL+V")) {
                        components.push_back(
                            std::make_shared<GUIVoltageSource>("V" + std::to_string(sources))
                        );
                        addingComponent = &(*components.back());
                        action = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Current source", "CTRL+J", false, false)) {}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Wire", "CTRL+W", false, false)) {
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
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Simulate"))
            {
                if (ImGui::MenuItem("Steady state analysis")) {
                    std::cout << circuit << std::endl;
                }
                if (ImGui::MenuItem("Transient analysis", NULL, false, false)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        window.clear(sf::Color(148, 143, 129));
        
        // draw components

        for ( auto it : components ) {
            window.draw(*it);
        }

        window.draw(lines);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;

}