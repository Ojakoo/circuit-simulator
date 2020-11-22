#include <stdlib.h>

#include "circuit_simulator_gui.hpp"


CircuitSimulatorGUI::CircuitSimulatorGUI(int width,int height, const std::string &title)
            : sf::RenderWindow(sf::VideoMode(width, height), title) { 
                this->setFramerateLimit(60);
                ImGui::SFML::Init(*this);
            }


TerminalType CircuitSimulatorGUI::DetermineTerminal(const sf::FloatRect bounds, const int rot, const sf::Vector2f mouse) const {
    TerminalType terminal;
    if (rot == 90 || rot == 270) {  // components rotation is vertical
        const float top = bounds.top;
        const float bottom = top + bounds.height;
        if (rot == 90) {  // rotation is 90 degrees
            if ( abs(mouse.y - top) > abs(mouse.y - bottom) ) {
                terminal = OUTPUT;
            } else {
                terminal = INPUT;
            }
        } else {  // rotation is 270 degrees
            if ( abs(mouse.y - top) > abs(mouse.y - bottom) ) {
                terminal = INPUT;
            } else {
                terminal = OUTPUT;
            }
        }
    } else { // components rotation is horizontal
        const float left = bounds.left;
        const float right = left + bounds.width;
        if ( rot == 0) {  // rotation is 0 degrees
            if ( abs(mouse.x - left) > abs(mouse.x - right) ) {
                terminal = OUTPUT;
            } else {
                terminal = INPUT;
            }
        } else {  // rotation is 180 degrees
            if ( abs(mouse.x - left) > abs(mouse.x - right) ) {
                terminal = INPUT;
            } else {
                terminal = OUTPUT;
            }
        }
    }
    return terminal;
}

void CircuitSimulatorGUI::ProcessEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        switch ( event.type )
        {
            case sf::Event::Closed:
                close();
                break;
        
            case sf::Event::Resized:
                view_.setSize(event.size.width, event.size.height);
                view_.zoom(zoom_);
                setView(view_);
                break;
            
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2f mouse = mapPixelToCoords(
                        sf::Mouse::getPosition(*this)
                    );

                    bool clicked_component = false;
                    for (auto it = components_.begin(); it != components_.end(); it++) {
                        const sf::FloatRect bounds = (*it)->getGlobalBounds();
                        if (bounds.contains(mouse)) {
                            switch ( action_ ) {
                                case MOVING_COMPONENT:
                                    if (movingComponent_) {
                                        // already moving component
                                        movingComponent_ = nullptr;
                                    } else {
                                        movingComponent_ = *it;
                                    }
                                    break;
                                case ROTATING_COMPONENT:
                                    (*it)->rotate(90);
                                    break;
                                case DELETING_COMPONENT:
                                    circuit_.RemoveComponent((*it)->GetComponent());
                                    components_.erase(it);
                                    break;
                                default:
                                    break;
                            }
                            clicked_component = true;
                            TerminalType terminal = DetermineTerminal(bounds, int((*it)->getRotation()), mouse);
                            std::cout << (*it)->GetName() << ", Clicked terminal: ";
                            switch ( terminal ) {
                                case INPUT:
                                    std::cout << "INPUT";
                                    break;
                                case OUTPUT:
                                    std::cout << "OUTPUT";
                                    break;
                                default:
                                    break;
                            }
                            std::cout << std::endl;
                            break;
                            
                        }
                    }

                    if (addingWire_) {
                        addingWire_->resize(addingWire_->getVertexCount() + 1);
                        (*addingWire_)[addingWire_->getVertexCount() - 1].position = sf::Vector2f(mouse.x, mouse.y);
                        (*addingWire_)[addingWire_->getVertexCount() - 1].color = sf::Color(0, 0, 0);
                    }

                    if (addingComponent_) {
                        switch ( addingComponent_->GetType() ) {
                            case RESISTOR:
                                resistors_++;
                                break;
                            case CAPACITOR:
                                capacitors_++;
                                break;
                            case INDUCTOR:
                                inductors_++;
                                break;
                            default:
                                break;
                        }
                        circuit_.AddComponent(addingComponent_->GetComponent());
                        addingComponent_ = nullptr;
                        action_ = NO_ACTION;
                    }
                    
                    if (movingComponent_ && !clicked_component) {
                        // moving a component but mouse is not inside a sprite
                        movingComponent_ = nullptr;
                    }
                    if (!clicked_component) {
                        movingView_ = true;
                        oldPos_ = mouse;
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    // cancel all actions
                    action_ = NO_ACTION;
                    movingComponent_ = nullptr;
                    if (addingComponent_) {  // Remove the component being added
                        components_.pop_back();
                    }
                    addingComponent_ = nullptr;

                    if (addingWire_) {  // Remove the wire being added
                        wires_.pop_back();
                    }
                    addingWire_ = nullptr;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    movingView_ = false;
                }
                break;

            case sf::Event::MouseMoved:
                {
                    const sf::Vector2f newPos = mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                    );

                    if (!movingView_) {
                        if (movingComponent_)
                            movingComponent_->setPosition(newPos);
                        
                        if (addingComponent_) {
                            addingComponent_->setPosition(newPos);
                        }
                        if (addingWire_) {
                            (*addingWire_)[addingWire_->getVertexCount() - 1].position = newPos;
                        }
                        break;
                    }

                    const sf::Vector2f deltaPos = oldPos_ - newPos;

                    view_.setCenter(view_.getCenter() + deltaPos);
                    setView(view_);

                    oldPos_ = mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                    );
                    break;
                }
            case sf::Event::MouseWheelScrolled:
                if (movingView_) break;

                if (event.mouseWheelScroll.delta <= -1) {
                    // Max zoom is 2.0
                    zoom_ = std::min(2.f, zoom_ + .1f);
                } else if (event.mouseWheelScroll.delta >= 1) {
                    // Min zoom is 0.5
                    zoom_ = std::max(.5f, zoom_ - .1f);
                }

                view_.setSize(getDefaultView().getSize());
                view_.zoom(zoom_);
                setView(view_);
                break;

            default:
                break; 
        }
    }
}


void CircuitSimulatorGUI::RenderMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O", false, false)) {}
            if (ImGui::MenuItem("Save", "CTRL+S", false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Close")) {
                close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Add component.."))
            {
                if (ImGui::MenuItem("Resistor", "R")) {
                    components_.push_back(
                        std::make_shared<GUIResistor>("R" + std::to_string(resistors_))
                    );
                    addingComponent_ = components_.back();
                    action_ = ADDING_COMPONENT;
                }
                if (ImGui::MenuItem("Capacitor", "C")) {
                    components_.push_back(
                        std::make_shared<GUICapacitor>("C" + std::to_string(capacitors_))
                    );
                    addingComponent_ = components_.back();
                    action_ = ADDING_COMPONENT;
                }
                if (ImGui::MenuItem("Inductor", "L")) {
                    components_.push_back(
                        std::make_shared<GUIInductor>("L" + std::to_string(inductors_))
                    );
                    addingComponent_ = components_.back();
                    action_ = ADDING_COMPONENT;
                }
                if (ImGui::MenuItem("Voltage source", "V")) {
                    components_.push_back(
                        std::make_shared<GUIVoltageSource>("V" + std::to_string(sources_))
                    );
                    addingComponent_ = components_.back();
                    action_ = ADDING_COMPONENT;
                }
                if (ImGui::MenuItem("Current source", "J", false, false)) {}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Wire", "W")) {
                wires_.push_back(
                    std::make_shared<GUIWire>()
                );
                addingWire_ = wires_.back();
                (*addingWire_)[addingWire_->getVertexCount() - 1].color = sf::Color(0, 0, 0);
                action_ = DRAWING_WIRE;
            }
            if (ImGui::MenuItem("Rotate", "R")) {
                action_ = ROTATING_COMPONENT;
            }
            if (ImGui::MenuItem("Move", "M")) {
                action_ = MOVING_COMPONENT;
            }
            if (ImGui::MenuItem("Delete", "D")) {
                action_ = DELETING_COMPONENT;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Simulate"))
        {
            if (ImGui::MenuItem("Steady state analysis")) {
                std::cout << circuit_ << std::endl;
            }
            if (ImGui::MenuItem("Transient analysis", NULL, false, false)) {}
            ImGui::EndMenu();
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 150);

        switch ( action_) {
            case MOVING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Moving component");
                break;
            case DELETING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Deleting component");
                break;
            case ADDING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Adding component");
                break;
            case ROTATING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Rotating component");
                break;
            case DRAWING_WIRE:
                ImGui::TextColored(ImVec4(1,1,0,1), "Drawing wire");
                break;
            default:
                break;
        }
        ImGui::EndMainMenuBar();
    }
}


void CircuitSimulatorGUI::DrawComponents() {
    clear(sf::Color(148, 143, 129));

    // draw components
    for ( auto it : components_ ) {
        draw(*it);
    }

    // draw wires
    for (auto it : wires_) {
        draw(*it);
    }
}


void CircuitSimulatorGUI::main_loop() {
    while (isOpen()) {
        ProcessEvents();
        ImGui::SFML::Update(*this, deltaClock_.restart());
        RenderMenuBar();
        DrawComponents();
        ImGui::SFML::Render(*this);
        display();
    }
}