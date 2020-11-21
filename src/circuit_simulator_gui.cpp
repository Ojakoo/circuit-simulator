#include "circuit_simulator_gui.hpp"


CircuitSimulatorGUI::CircuitSimulatorGUI(int width,int height, const std::string &title)
            : sf::RenderWindow(sf::VideoMode(width, height), title) { 
                this->setFramerateLimit(60);
                ImGui::SFML::Init(*this);
            }


void CircuitSimulatorGUI::main_loop() {
    while (isOpen()) {
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
                            if ((*it)->getGlobalBounds().contains(mouse)) {
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
                                break;
                            }
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
                        if (addingComponent_) {
                            components_.pop_back();
                        }
                        addingComponent_ = nullptr;
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

        ImGui::SFML::Update(*this, deltaClock_.restart());

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
                    if (ImGui::MenuItem("Resistor", "CTRL+R")) {
                        components_.push_back(
                            std::make_shared<GUIResistor>("R" + std::to_string(resistors_))
                        );
                        addingComponent_ = &(*components_.back());
                        action_ = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Capacitor", "CTRL+C")) {
                        components_.push_back(
                            std::make_shared<GUICapacitor>("C" + std::to_string(capacitors_))
                        );
                        addingComponent_ = &(*components_.back());
                        action_ = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Inductor", "CTRL+I")) {
                        components_.push_back(
                            std::make_shared<GUIInductor>("L" + std::to_string(inductors_))
                        );
                        addingComponent_ = &(*components_.back());
                        action_ = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Voltage source", "CTRL+V")) {
                        components_.push_back(
                            std::make_shared<GUIVoltageSource>("V" + std::to_string(sources_))
                        );
                        addingComponent_ = &(*components_.back());
                        action_ = ADDING_COMPONENT;
                    }
                    if (ImGui::MenuItem("Current source", "CTRL+J", false, false)) {}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Wire", "CTRL+W", false, false)) {
                    action_ = DRAWING_WIRE;
                }
                if (ImGui::MenuItem("Rotate", "CTRL+R")) {
                    action_ = ROTATING_COMPONENT;
                }
                if (ImGui::MenuItem("Move", "CTRL+M")) {
                    action_ = MOVING_COMPONENT;
                }
                if (ImGui::MenuItem("Delete", "CTRL+D")) {
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
            ImGui::EndMainMenuBar();
        }

        clear(sf::Color(148, 143, 129));

        // draw components

        for ( auto it : components_ ) {
            draw(*it);
        }

        //draw(lines);

        ImGui::SFML::Render(*this);
        display();
    }
}