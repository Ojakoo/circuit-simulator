#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <set>

#include "ImGuiFileBrowser.h"

#include "circuit_simulator_gui.hpp"


const float distance(sf::Vector2f &a, sf::Vector2f &b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

const float dot(const sf::Vector2f &a, const sf::Vector2f &b) {
    return a.x * b.x + a.y * b.y;
}

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

void CircuitSimulatorGUI::AddingWire(std::shared_ptr<GUIWire> wire) {
    wires_.push_back(wire);
    (*wire)[0].color = sf::Color(0, 0, 0);
    addingWire_ = wire;
    action_ = DRAWING_WIRE;
}

std::list<std::shared_ptr<GUIWire>>::const_iterator CircuitSimulatorGUI::WireClick(sf::Vector2f mouse) const {
    auto it = wires_.begin();
    for ( ; it != wires_.end(); it++) {
        for (int i = 0; i < (*it)->getVertexCount() - 1; i++) {
            auto P_1 = (*(*it))[i].position;
            auto P_2 = (*(*it))[i + 1].position;
            // https://stackoverflow.com/a/1501725
            float d;
            float l2 = std::pow(P_2.x - P_1.x, 2) + std::pow(P_2.y - P_1.y, 2);
            if (l2 == 0.0) {
                d = distance(mouse, P_1);
            } else {
                const float t = std::max(0.0f, std::min(1.0f, dot(mouse - P_1, P_2 - P_1) / l2));
                sf::Vector2f projection = P_1 + t * (P_2 - P_1);
                d = distance(mouse, projection);
            }
            if (abs(d) <= 10 ) {
                // found one!
                return it;
            }
        }
    }
    return it;
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
    if (addingGround_) {
        grounds_.pop_back();
    }
    addingWire_ = nullptr;
    editingComponent_ = nullptr;
    addingGround_ = nullptr;
}

void CircuitSimulatorGUI::UpdateHelperLines(sf::Vector2i closest) {
    helper_lines_[0].position = sf::Vector2f(0, closest.y);
    helper_lines_[1].position = sf::Vector2f(this->getSize().x,closest.y);
    helper_lines_[2].position = sf::Vector2f(closest.x, 0);
    helper_lines_[3].position = sf::Vector2f(closest.x, this->getSize().y);
}

void CircuitSimulatorGUI::Reset() {
    CancelAllActions();
    circuit_ = Circuit();
    resistors_ = 0;
    inductors_ = 0;
    capacitors_ = 0;
    sources_ = 0;
    nodes_ = 0;
    components_.clear();
    wires_.clear();
    grounds_.clear();
    zoom_ = 1;
    popup_value_ = 0.0;
}

void CircuitSimulatorGUI::LoadCircuit(std::string &file) {
    Reset();

    std::ifstream ifstr(file);

    if (ifstr.rdstate() & (ifstr.failbit | ifstr.badbit)) {
        throw std::runtime_error("Failed to read netlist file.");
    }

    std::set<std::string> c = {"R", "L", "C", "V"};

    while( !ifstr.eof() ) {
        std::string line;
        std::getline(ifstr, line);
        std::stringstream iss(line);
        if (line.length() == 0) continue;
        std::string type;
        std::string name;
        std::string input_node;
        std::string output_node;
        float value;
        float x;
        float y;
        float rot;
        iss >> type;
        if (!iss) {
            // Check that reading succeeded
            throw std::runtime_error("Error while reading netlist file.");
        }
        if (c.find(type) != c.end()) {
            // Normal component
            iss >> name >> input_node >> output_node >> value >> x >> y >> rot;
            auto in = circuit_.AddNode(input_node);
            auto out = circuit_.AddNode(output_node);
            if ( type == "R" ) {
                auto r = std::make_shared<GUIResistor>(name, value, in, out);
                resistors_++;
                components_.push_back(r);
                circuit_.AddComponent(r->GetComponent());
            } else if ( type == "L" ) {
                auto l = std::make_shared<GUIInductor>(name, value, in, out);
                inductors_++;
                components_.push_back(l);
                circuit_.AddComponent(l->GetComponent());
            } else if ( type == "C" ) {
                auto cap = std::make_shared<GUICapacitor>(name, value, in, out);
                capacitors_++;
                components_.push_back(cap);
                circuit_.AddComponent(cap->GetComponent());
            } else if ( type == "V" ) {
                auto v = std::make_shared<GUIVoltageSource>(name, value, in, out);
                sources_++;
                components_.push_back(v);
                circuit_.AddComponent(v->GetComponent());
            } else {
                throw std::runtime_error("Invalid component type found in netlist.");
            }
            auto comp = components_.back();
            comp->setRotation(rot);
            comp->setPosition(x, y);
        } else if (type == "W") {
            // Wire
            std::string node;
            int verticies;
            iss >> node >> verticies;
            auto w = std::make_shared<GUIWire>(circuit_);
            auto n = circuit_.AddNode(node);
            wires_.push_back(w);
            w->SetNode(n);
            w->resize(verticies);
            for (int i = 0; i < verticies; i++) {
                std::string line;
                std::getline(ifstr, line);
                std::stringstream iss(line);
                float x, y;
                iss >> x >> y;
                (*w)[i].position = sf::Vector2f(x, y);
                (*w)[i].color = sf::Color::Black;
            }
        } else if (type == "G") {
            // Ground
            std::string node;
            float x, y;
            iss >> node >> x >> y;
            auto n = circuit_.AddNode(node);
            auto g = std::make_shared<GUIGround>();
            g->SetNode(n);
            g->setPosition(x, y);
            grounds_.push_back(g);
        }
    }

    ifstr.close();
}

void CircuitSimulatorGUI::SaveCircuit(std::string &file) {
    std::ofstream save_file;
    save_file.open(file);
    for ( auto comp : components_ ) {
        switch (comp->GetType()) {
            case RESISTOR:
                save_file << "R ";
                break;
            case CAPACITOR:
                save_file << "C ";
                break;
            case INDUCTOR:
                save_file << "C ";
                break;
            case VOLTAGE_SOURCE:
                save_file << "V ";
                break;
            default:
                break;
        }
        auto in = comp->GetTerminalNode(INPUT);
        auto out = comp->GetTerminalNode(OUTPUT);
        save_file << comp->GetName() << " "
                  << (in ? in->GetName() : "-") << " "
                  << (out ? out->GetName() : "-") << " "
                  << comp->GetValue() << " "
                  << comp->getPosition().x << " "
                  << comp->getPosition().y << " "
                  << comp->getRotation() << std::endl;
    }
    for ( auto wire : wires_ ) {
        auto node = wire->GetNode();
        save_file << "W " << (node ? node->GetName() : "-") << " " << wire->getVertexCount() << std::endl;
        for (int i = 0; i < wire->getVertexCount(); i++) {
            save_file << (*wire)[i].position.x << " " << (*wire)[i].position.y << std::endl;
        } 
    }
    for ( auto gnd : grounds_ ) {
        save_file << "G " << gnd->GetNode()->GetName() << " "
                          << gnd->getPosition().x << " "
                          << gnd->getPosition().y << std::endl;
    }
    save_file.close();
    std::cout << "Netlist saved into : " << file << "." << std::endl;
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
                    if (editingComponent_ && action_ == EDIT_VALUE) break;
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
                                    }
                                    break;
                                case ROTATING_COMPONENT:
                                    (*it)->rotate(90);
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

                    if (action_ == ADDING_GROUND && addingGround_) {
                        if (clicked_component) {
                            auto rot = clicked_component->getRotation();
                            auto pair = TerminalClick(bounds, rot, mouse);
                            if ( !(clicked_component->GetTerminalNode(pair.first)) ) {
                                auto node = circuit_.AddNode("N" + std::to_string(nodes_));
                                nodes_++;
                                clicked_component->ConnectNodeToTerminal(pair.first, node);
                            }
                            addingGround_->SetNode(clicked_component->GetTerminalNode(pair.first));
                            addingGround_->setPosition(pair.second);
                            addingGround_ = nullptr;
                            CancelAllActions();
                        } else {
                            // check if we clicked on wire
                            auto it = WireClick(mouse);
                            if (it != wires_.end()) {
                                if ( !(*it)->GetNode() ) {
                                    auto node = circuit_.AddNode("N" + std::to_string(nodes_));
                                    nodes_++;
                                    (*it)->SetNode(node);
                                }
                                addingGround_->SetNode((*it)->GetNode());
                                addingGround_ = nullptr;
                                CancelAllActions();
                            }
                        }
                    }

                    if ( action_ == DELETING_ELEMENT  && !clicked_component ) {
                        auto it = WireClick(mouse);
                        if (it != wires_.end()) {
                            wires_.erase(it);
                        } else {
                            for (auto it = grounds_.begin(); it != grounds_.end(); it++) {
                                if ((*it)->getGlobalBounds().contains(mouse)) {
                                    grounds_.erase(it);
                                    break;
                                }
                            }
                        }
                    }

                    if ((addingWire_ && action_ == DRAWING_WIRE)) {
                        bool skip = false;
                        int count = addingWire_->getVertexCount();
                        if (clicked_component) {
                            auto rot = clicked_component->getRotation();
                            auto pair = TerminalClick(bounds, rot, mouse);
                            (*addingWire_)[count - 1].position = pair.second;
                            auto node = clicked_component->GetComponent()->GetTerminalNode(pair.first);
                            if (node) {
                                // component already has a node at that terminal and
                                if (addingWire_->GetNode() == node) {
                                    // closed loop
                                    skip = true;
                                    wires_.remove(addingWire_);
                                    CancelAllActions();
                                } else {
                                    addingWire_->SetNode(node);
                                }
                            } else { // component doesn't have a node at that terminal
                                if (addingWire_->GetNode()) {
                                    // The wire already has a node
                                    node = addingWire_->GetNode();
                                } else {
                                    // the wire doesn't have a node
                                    node = circuit_.AddNode("N" + std::to_string(nodes_));
                                    nodes_++;
                                    addingWire_->SetNode(node);
                                }
                                clicked_component->ConnectNodeToTerminal(pair.first, node);
                            }
                            if (!skip) {
                                clicked_component->ConnectWire(pair.first);
                                addingWire_->ConnectComponent(clicked_component, pair.first);
                            }
                            //clicked_component->SetTerminalRects(pair.first, pair.second);
                        }
                        if (!skip) {
                            addingWire_->resize(count + 1);
                            (*addingWire_)[count].position = sf::Vector2f(mouse.x, mouse.y);
                            (*addingWire_)[count].color = sf::Color(0, 0, 0);
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
                            case VOLTAGE_SOURCE:
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
                            if (addingWire_->getVertexCount() >= 2) {
                                auto last = (*addingWire_)[addingWire_->getVertexCount() - 2].position;
                                if (abs(last.x - closest.x) >= abs(last.y - closest.y)) {
                                    closest.y = last.y;
                                } else {
                                    closest.x = last.x;
                                }
                            }
                            UpdateHelperLines(closest);
                            (*addingWire_)[addingWire_->getVertexCount() - 1].position = sf::Vector2f(closest.x, closest.y);
                            
                        }
                        if (addingGround_) {
                            sf::Vector2i closest = MapCoordsToClosest(sf::Vector2i(int(newPos.x), int(newPos.y)));
                            addingGround_->setPosition(closest.x, closest.y);
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
                        AddingWire(std::make_shared<GUIWire>(circuit_));
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
                } else if (event.key.code == sf::Keyboard::G && event.key.control) {
                    if (!addingGround_ && action_ != ADDING_GROUND) {
                        action_ = ADDING_GROUND;
                        grounds_.push_back(std::make_shared<GUIGround>());
                        addingGround_ = grounds_.back();
                    }
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
                AddingWire(std::make_shared<GUIWire>(circuit_));
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
            if (ImGui::MenuItem("Ground", "CTRL+G")) {
                if (!addingGround_ && action_ != ADDING_GROUND) {
                    action_ = ADDING_GROUND;
                    grounds_.push_back(std::make_shared<GUIGround>());
                    addingGround_ = grounds_.back();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Simulate"))
        {
            if (ImGui::MenuItem("Steady state analysis")) {
                std::cout << circuit_ << std::endl;
                for (auto it: circuit_.GetNodes()) {
                    std::cout << *(it.second) << std::endl;
                }
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
            case ADDING_GROUND:
                ImGui::TextColored(ImVec4(1,1,0,1), "Adding ground");
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
        //it->DrawTerminalRects(*this);
    }

    // draw wires
    for (auto it : wires_) {
        draw(*it);
        it->DrawInfo(*this);
    }

    // draw ground
    for (auto it : grounds_ ) {
        it->draw(*this);
    }

    // draw(lines);
    // draw helper lines
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