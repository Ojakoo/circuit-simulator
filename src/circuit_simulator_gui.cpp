#include <stdlib.h>
#include <math.h>

#include "ImGuiFileBrowser.h"

#include "circuit_simulator_gui.hpp"


sf::Vector2i MapCoordsToClosest(sf::Vector2i coords) {
    int offset_x = int(coords.x) % GRID_SIZE;
    if (offset_x > (GRID_SIZE / 2))
        coords.x += GRID_SIZE - offset_x;
    else
        coords.x -= offset_x;

    int offset_y = int(coords.y) % GRID_SIZE;
    if (offset_y > (GRID_SIZE / 2))
        coords.y += GRID_SIZE - offset_y;
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


void CircuitSimulatorGUI::CancelAllActions() {
    action_ = NO_ACTION;
    movingComponent_ = nullptr;
    if (addingComponent_) {  // Remove the component being added
        components_.pop_back();
    }
    addingComponent_ = nullptr;

    if (addingWire_) {
        if (addingWire_->getVertexCount() == 2) {
            // Remove the wire being added if it is too short
            wires_.pop_back();
        } else {
            addingWire_->resize(addingWire_->getVertexCount() - 1);
        }
    }
    addingWire_ = nullptr;
    editingComponent_ = nullptr;
}

void CircuitSimulatorGUI::UpdateHelperLines(sf::Vector2i closest) {
    helper_lines_[0].position = sf::Vector2f(0, closest.y);
    helper_lines_[1].position = sf::Vector2f(this->getSize().x,closest.y);
    helper_lines_[2].position = sf::Vector2f(closest.x, 0);
    helper_lines_[3].position = sf::Vector2f(closest.x, this->getSize().y);
}


void CircuitSimulatorGUI::LoadCircuit(std::string &file) {
    std::cout << file << std::endl;
}

void CircuitSimulatorGUI::SaveCircuit(std::string &file) {
    std::cout << file << std::endl;
}


CircuitSimulatorGUI::CircuitSimulatorGUI(int width,int height, const std::string &title)
            : sf::RenderWindow(sf::VideoMode(width, height), title) { 
                this->setFramerateLimit(60);
                ImGui::SFML::Init(*this);
                lines = sf::VertexArray(sf::Lines, 200);

                // FOR DEBUGGING
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

                helper_lines_[0].color = sf::Color(197, 206, 219, 100);
                helper_lines_[1].color = sf::Color(197, 206, 219, 100);
                helper_lines_[2].color = sf::Color(197, 206, 219, 100);
                helper_lines_[3].color = sf::Color(197, 206, 219, 100);
            }


std::pair<TerminalType, sf::Vector2f> CircuitSimulatorGUI::TerminalClick(const sf::FloatRect bounds, const int rot, const sf::Vector2f mouse) const {
    TerminalType terminal;
    sf::Vector2f coords;
    if (rot == 90 || rot == 270) {  // components rotation is vertical
        const float top = bounds.top;
        const float bottom = top + bounds.height;
        if (rot == 90) {  // rotation is 90 degrees
            if ( abs(mouse.y - top) > abs(mouse.y - bottom) ) {
                terminal = OUTPUT;
                coords.x = bounds.left + bounds.width / 2;
                coords.y = bottom;
            } else {
                terminal = INPUT;
                coords.x = bounds.left + bounds.width / 2;
                coords.y = top;
            }
        } else {  // rotation is 270 degrees
            if ( abs(mouse.y - top) > abs(mouse.y - bottom) ) {
                terminal = INPUT;
                coords.x = bounds.left + bounds.width / 2;
                coords.y = bottom;
            } else {
                terminal = OUTPUT;
                coords.x = bounds.left + bounds.width / 2;
                coords.y = top;
            }
        }
    } else { // components rotation is horizontal
        const float left = bounds.left;
        const float right = left + bounds.width;
        if ( rot == 0) {  // rotation is 0 degrees
            if ( abs(mouse.x - left) > abs(mouse.x - right) ) {
                terminal = OUTPUT;
                coords.x = right;
                coords.y = bounds.top + bounds.height/2;
            } else {
                terminal = INPUT;
                coords.x = left;
                coords.y = bounds.top + bounds.height/2;
            }
        } else {  // rotation is 180 degrees
            if ( abs(mouse.x - left) > abs(mouse.x - right) ) {
                terminal = INPUT;
                coords.x = right;
                coords.y = bounds.top + bounds.height/2;
            } else {
                terminal = OUTPUT;
                coords.x = left;
                coords.y = bounds.top + bounds.height/2;
            }
        }
    }
    return std::make_pair(terminal, coords);
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
                    if (editingComponent_ && action_ == EDIT_VALUE) return;
                    sf::Vector2f mouse = mapPixelToCoords(
                        sf::Mouse::getPosition(*this)
                    );

                    sf::FloatRect bounds;

                    std::shared_ptr<GUIComponent> clicked_component = nullptr;

                    bool deleted = false;

                    for (auto it = components_.begin(); it != components_.end(); it++) {
                        if (deleted) break;
                        bounds = (*it)->getGlobalBounds();
                        if (bounds.contains(mouse)) {
                            switch ( action_ ) {
                                case MOVING_COMPONENT:
                                    if (movingComponent_) {
                                        // already moving component
                                        movingComponent_ = nullptr;
                                    } else {
                                        movingComponent_ = *it;
                                        movingComponent_->Disconnect();
                                    }
                                    break;
                                case ROTATING_COMPONENT:
                                    (*it)->rotate(90);
                                    (*it)->Disconnect();
                                    break;
                                case DELETING_ELEMENT:
                                    circuit_.RemoveComponent((*it)->GetComponent());
                                    components_.erase(it);
                                    deleted = true;
                                    break;
                                case EDIT_VALUE:
                                    editingComponent_ = *it;
                                    popup_value_ = editingComponent_->GetValue();
                                    break;
                                default:
                                    break;
                            }
                            if (!deleted) clicked_component = *it;
                            break;
                        }
                    }
                    if ( action_ == DELETING_ELEMENT  && !clicked_component ) {
                        
                        bool closeToWire = false;
                        for (auto it = wires_.begin(); it != wires_.end(); it++) {
                            /*
                            if ((*it)->getBounds().contains(mouse)) {  // Proceed if mouse is relatively close to the wire
                            DOES NOT WORK WITH HORIZONTAL OR VERTICAL LINES!
                            */
                            
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
                                    closeToWire = true;
                                    break;
                                }
                            }
                            if (closeToWire) {
                                wires_.erase(it);
                                break;
                            }
                        }
                    }

                    if (addingWire_ && action_ == DRAWING_WIRE) {
                        int count = addingWire_->getVertexCount();
                        if (clicked_component) {
                            auto rot = clicked_component->getRotation();
                            auto pair = TerminalClick(bounds, rot, mouse);
                            (*addingWire_)[count - 1].position = pair.second;
                            clicked_component->SetTerminalRects(pair.first, pair.second);
                        }
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
                            case DC_VOLTAGE_SOURCE:
                                sources_++;
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
                    CancelAllActions();
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (editingComponent_ && action_ == EDIT_VALUE) break;
                if (event.mouseButton.button == sf::Mouse::Left) {
                    movingView_ = false;
                }
                break;

            case sf::Event::MouseMoved:
                if (editingComponent_ && action_ == EDIT_VALUE) break;
                {
                    const sf::Vector2f newPos = mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y)
                    );

                    const sf::Vector2f deltaPos = oldPos_ - newPos;

                    if (!movingView_) {
                        if (movingComponent_) {
                            sf::Vector2i closest = MapCoordsToClosest(sf::Vector2i(int(newPos.x), int(newPos.y)));
                            movingComponent_->setPosition(closest.x, closest.y);
                        }
                        if (addingWire_) {
                            sf::Vector2i closest = MapCoordsToClosest(sf::Vector2i(int(newPos.x), int(newPos.y)));
                            UpdateHelperLines(closest);
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
                if (editingComponent_ && action_ == EDIT_VALUE) break;
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

            case sf::Event::KeyPressed:
                if (editingComponent_ && action_ == EDIT_VALUE) break;
                if (event.key.code == sf::Keyboard::M && event.key.control) {  // Move
                    action_ = MOVING_COMPONENT;
                } else if (event.key.code == sf::Keyboard::F && event.key.control) {  // Rotate or flip
                    action_ = ROTATING_COMPONENT;
                } else if (event.key.code == sf::Keyboard::D && event.key.control) {
                    action_ = DELETING_ELEMENT;
                } else if (event.key.code == sf::Keyboard::W && event.key.control) {
                    if (!addingWire_ && action_ != DRAWING_WIRE) {
                        wires_.push_back(
                            std::make_shared<GUIWire>()
                        );
                        addingWire_ = wires_.back();
                        (*addingWire_)[0].color = sf::Color(0, 0, 0);
                        action_ = DRAWING_WIRE;
                    }
                } else if (event.key.code == sf::Keyboard::Escape) {
                    CancelAllActions();
                } else if (event.key.code == sf::Keyboard::R && event.key.control) {
                    if (!addingComponent_ && action_ != ADDING_COMPONENT) {
                        AddingComponent(
                            std::make_shared<GUIResistor>("R" + std::to_string(resistors_))
                        );
                    }
                } else if (event.key.code == sf::Keyboard::C && event.key.control) {
                    if (!addingComponent_ && action_ != ADDING_COMPONENT) {
                        AddingComponent(
                            std::make_shared<GUICapacitor>("C" + std::to_string(capacitors_))
                        );
                    }
                } else if (event.key.code == sf::Keyboard::L && event.key.control) {
                    if (!addingComponent_ && action_ != ADDING_COMPONENT) {
                        AddingComponent(
                            std::make_shared<GUIInductor>("L" + std::to_string(inductors_))
                        );
                    }
                } else if (event.key.code == sf::Keyboard::V && event.key.control) {
                    if (!addingComponent_ && action_ != ADDING_COMPONENT) {
                        AddingComponent(
                            std::make_shared<GUIVoltageSource>("V" + std::to_string(sources_))
                        );
                    }
                } else if (event.key.code == sf::Keyboard::E && event.key.control) {
                    action_ = EDIT_VALUE;
                } else if (event.key.code == sf::Keyboard::O && event.key.control) {
                    
                } else if (event.key.code == sf::Keyboard::S && event.key.control) {
                    
                }
                break;

            default:
                break; 
        }
    }
}


void CircuitSimulatorGUI::RenderMenuBar() {
    bool open = false, save = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                open = true;
            }
            if (ImGui::MenuItem("Save", "CTRL+S")) {
                save = true;
            }
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
                    AddingComponent(
                        std::make_shared<GUIResistor>("R" + std::to_string(resistors_))
                    );
                }
                if (ImGui::MenuItem("Capacitor", "CTRL+C")) {
                    AddingComponent(
                        std::make_shared<GUICapacitor>("C" + std::to_string(capacitors_))
                    );
                }
                if (ImGui::MenuItem("Inductor", "CTRL+L")) {
                    AddingComponent(
                        std::make_shared<GUIInductor>("L" + std::to_string(inductors_))
                    );
                }
                if (ImGui::MenuItem("Voltage source", "CTRL+V")) {
                    AddingComponent(
                        std::make_shared<GUIVoltageSource>("V" + std::to_string(sources_))
                    );
                }
                if (ImGui::MenuItem("Current source", "CTRL+J", false, false)) {}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Wire", "CTRL+W")) {
                wires_.push_back(
                    std::make_shared<GUIWire>()
                );
                addingWire_ = wires_.back();
                (*addingWire_)[0].color = sf::Color(0, 0, 0);
                action_ = DRAWING_WIRE;
            }
            if (ImGui::MenuItem("Flip", "CTRL+F")) {
                action_ = ROTATING_COMPONENT;
            }
            if (ImGui::MenuItem("Move", "CTRL+M")) {
                action_ = MOVING_COMPONENT;
            }
            if (ImGui::MenuItem("Delete", "CTRL+D")) {
                action_ = DELETING_ELEMENT;
            }
            if (ImGui::MenuItem("Value", "CTRL+E")) {
                action_ = EDIT_VALUE;
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
            case EDIT_VALUE:
                ImGui::TextColored(ImVec4(1,1,0,1), "Edit value...");
                break;
            default:
                break;
        }
        ImGui::EndMainMenuBar();
    }
    //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
    if(open)
        ImGui::OpenPopup("Open File");
    if(save)
        ImGui::OpenPopup("Save File");
        
    /* Optional third parameter. Support opening only compressed rar/zip files. 
     * Opening any other file will show error, return false and won't close the dialog.
     */
    if(file_dialog_.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt"))
    {
        LoadCircuit(file_dialog_.selected_path);
        //std::cout << file_dialog_.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
        //std::cout << file_dialog_.selected_path << std::endl;    // The absolute path to the selected file
    }
    if(file_dialog_.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt"))
    {
        SaveCircuit(file_dialog_.selected_path);
        //std::cout << file_dialog_.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
        //std::cout << file_dialog_.selected_path << std::endl;    // The absolute path to the selected file
        //std::cout << file_dialog_.ext << std::endl;              // Access ext separately (For SAVE mode)
        //Do writing of files based on extension here
    }
}

void CircuitSimulatorGUI::RenderPopup() {
    if (editingComponent_ && action_ == EDIT_VALUE) {
        ImGui::OpenPopup("Edit value");
        if (ImGui::BeginPopupModal("Edit value", NULL)) {
            std::stringstream str;
            std::string val = std::to_string(editingComponent_->GetValue());
            str << "Edit the value of " << editingComponent_->GetName() << "\n";
            ImGui::Text("%s", str.str().c_str());
            ImGui::InputFloat("Value", &popup_value_, 0.0f, 0.0f, "%e");
            if (ImGui::Button("OK")) {
                if (popup_value_ > 0 || editingComponent_->GetClass() == ACTIVE) {
                    editingComponent_->SetValue(popup_value_);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                editingComponent_ = nullptr;
            }
            ImGui::EndPopup();
        }
    }
}


void CircuitSimulatorGUI::DrawComponents() {
    clear(sf::Color(148, 143, 129));

    // draw components
    for ( auto it : components_ ) {
        draw(*it);
        it->DrawInfo(*this);
        it->DrawTerminalRects(*this);
    }

    // draw wires
    for (auto it : wires_) {
        draw(*it);
    }

    // draw(lines);
    if (action_ == DRAWING_WIRE) {
        draw(helper_lines_);
    }
}


void CircuitSimulatorGUI::main_loop() {
    while (isOpen()) {
        ProcessEvents();
        ImGui::SFML::Update(*this, deltaClock_.restart());
        RenderMenuBar();
        DrawComponents();
        RenderPopup();
        ImGui::SFML::Render(*this);
        display();
    }
}