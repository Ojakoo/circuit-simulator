#include <stdlib.h>
#include <math.h>

#include "circuit_simulator_gui.hpp"


sf::Vector2i MapCoordsToClosest(sf::Vector2i coords, int grid_size) {
    int offset_x = int(coords.x) % grid_size;
    if (offset_x > (grid_size / 2))
        coords.x += grid_size - offset_x;
    else
        coords.x -= offset_x;

    int offset_y = int(coords.y) % grid_size;
    if (offset_y > (grid_size / 2))
        coords.y += grid_size - offset_y;
    else
        coords.y -= offset_y;
    return coords;
}


void CircuitSimulatorGUI::AddingComponent(std::shared_ptr<GUIComponent> component) {
    components_.push_back(component);
    addingComponent_ = component;
    movingComponent_ = component;
    action_ = ADDING_COMPONENT;
}


CircuitSimulatorGUI::CircuitSimulatorGUI(int width,int height, const std::string &title)
            : sf::RenderWindow(sf::VideoMode(width, height), title) { 
                this->setFramerateLimit(60);
                ImGui::SFML::Init(*this);
                lines = sf::VertexArray(sf::Lines, 200);

                // FOR DEBUGGING
                int k = 0;

                // vertical helper lines
                for ( int i = 0; i <= 640; i+=WIRE_GRID_SIZE, k+=2) {
                    lines[k].position = sf::Vector2f(i, 0);
                    lines[k].color = sf::Color(197, 206, 219);
                    lines[k + 1].position = sf::Vector2f(i, 480);
                    lines[k + 1].color = sf::Color(197, 206, 219);
                }

                //  horizontal helper lines
                for ( int j = 0; j <= 480; j+=WIRE_GRID_SIZE, k+=2 ) {
                    lines[k].position = sf::Vector2f(0, j);
                    lines[k].color = sf::Color(197, 206, 219);
                    lines[k + 1].position = sf::Vector2f(640, j);
                    lines[k + 1].color = sf::Color(197, 206, 219);
                }
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
                                case DELETING_ELEMENT:
                                    circuit_.RemoveComponent((*it)->GetComponent());
                                    components_.erase(it);
                                    break;
                                default:
                                    break;
                            }
                            clicked_component = true;
                            /*
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
                            */
                            break;
                            
                        }
                    }
                    if ( action_ == DELETING_ELEMENT ) {
                        for (auto it = wires_.begin(); it != wires_.end(); it++) {
                            if ((*it)->getBounds().contains(mouse)) {  // Proceed if mouse is relatively close to the wire
                                for (int i = 0; i < (*it)->getVertexCount() - 1; i++) {  // loop through every line formed by the vertex array
                                    sf::Vertex P_1 = (*(*it))[i];
                                    sf::Vertex P_2 = (*(*it))[i + 1];
                                    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
                                    float d = (
                                        abs((P_2.position.y - P_1.position.y) * mouse.x -
                                            (P_2.position.x - P_1.position.x) * mouse.y +
                                             P_2.position.x * P_1.position.y - P_2.position.y * P_1.position.x)
                                        / std::sqrt(std::pow(P_2.position.x - P_1.position.x, 2) + std::pow(P_2.position.y - P_1.position.y, 2))
                                    );
                                    if (abs(d) <= 10) {
                                        // Hit wire
                                        wires_.erase(it);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (addingWire_) {
                        int count = addingWire_->getVertexCount();
                        addingWire_->resize(count + 1);
                        (*addingWire_)[count].position = sf::Vector2f(mouse.x, mouse.y);
                        (*addingWire_)[count].color = sf::Color(0, 0, 0);
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
                        movingComponent_ = nullptr;
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
                        if (addingWire_->getVertexCount() == 2) {
                            wires_.pop_back();
                        } else {
                            addingWire_->resize(addingWire_->getVertexCount() - 1);
                        }
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

                    const sf::Vector2f deltaPos = oldPos_ - newPos;

                    if (!movingView_) {
                        if (movingComponent_) {
                            sf::Vector2i closest = MapCoordsToClosest(sf::Vector2i(int(newPos.x), int(newPos.y)), COMPONENT_GRID_SIZE);
                            movingComponent_->setPosition(closest.x, closest.y);
                        }
                        if (addingWire_) {
                            sf::Vector2i closest = MapCoordsToClosest(sf::Vector2i(int(newPos.x), int(newPos.y)), WIRE_GRID_SIZE);
                            (*addingWire_)[addingWire_->getVertexCount() - 1].position = sf::Vector2f(closest.x, closest.y);
                        }
                        
                    } else {
                        // moving view
                        view_.setCenter(view_.getCenter() + deltaPos);
                        setView(view_);
                    }

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
                    AddingComponent(
                        std::make_shared<GUIResistor>("R" + std::to_string(resistors_))
                    );
                }
                if (ImGui::MenuItem("Capacitor", "C")) {
                    AddingComponent(
                        std::make_shared<GUICapacitor>("C" + std::to_string(capacitors_))
                    );
                }
                if (ImGui::MenuItem("Inductor", "L")) {
                    AddingComponent(
                        std::make_shared<GUIInductor>("L" + std::to_string(inductors_))
                    );
                }
                if (ImGui::MenuItem("Voltage source", "V")) {
                    AddingComponent(
                        std::make_shared<GUIVoltageSource>("V" + std::to_string(sources_))
                    );
                }
                if (ImGui::MenuItem("Current source", "J", false, false)) {}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Wire", "W")) {
                wires_.push_back(
                    std::make_shared<GUIWire>()
                );
                addingWire_ = wires_.back();
                (*addingWire_)[0].color = sf::Color(0, 0, 0);
                action_ = DRAWING_WIRE;
            }
            if (ImGui::MenuItem("Rotate", "R")) {
                action_ = ROTATING_COMPONENT;
            }
            if (ImGui::MenuItem("Move", "M")) {
                action_ = MOVING_COMPONENT;
            }
            if (ImGui::MenuItem("Delete", "D")) {
                action_ = DELETING_ELEMENT;
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
                cursor_.loadFromSystem(sf::Cursor::Hand);
                setMouseCursor(cursor_);
                break;
            case DELETING_ELEMENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Deleting elements");
                break;
            case ADDING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Adding component");
                break;
            case ROTATING_COMPONENT:
                ImGui::TextColored(ImVec4(1,1,0,1), "Rotating component");
                cursor_.loadFromSystem(sf::Cursor::Hand);
                setMouseCursor(cursor_);
                break;
            case DRAWING_WIRE:
                ImGui::TextColored(ImVec4(1,1,0,1), "Drawing wire");
                cursor_.loadFromSystem(sf::Cursor::Cross);
                setMouseCursor(cursor_);
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

    // draw(lines);
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