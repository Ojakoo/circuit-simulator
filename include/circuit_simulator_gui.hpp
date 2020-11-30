#pragma once

#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"

#include "ImGuiFileBrowser.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "save_and_load.hpp"
#include "save_and_load.hpp"

#include "gui_components/gui_resistor.hpp"
#include "gui_components/gui_capacitor.hpp"
#include "gui_components/gui_inductor.hpp"
#include "gui_components/gui_voltage_source.hpp"
#include "gui_components/gui_wire.hpp"
#include "gui_components/gui_ground.hpp"

#include "circuit.hpp"

#define GRID_SIZE 20

enum GUIAction {
    NO_ACTION,
    MOVING_COMPONENT,
    ROTATING_COMPONENT,
    DELETING_ELEMENT,
    DRAWING_WIRE,
    ADDING_COMPONENT,
    EDIT_VALUE,
    ADDING_GROUND
};


class CircuitSimulatorGUI : public sf::RenderWindow {
    public:
        CircuitSimulatorGUI(int width, int height, const std::string &title);
        ~CircuitSimulatorGUI() = default;
        
        void main_loop();

        void ProcessEvents();

        void RenderMenuBar();

        void RenderPopup();

        void DrawComponents();

        void AddingComponent(std::shared_ptr<GUIComponent> component);

        void AddingWire(std::shared_ptr<GUIWire> wire);

        void CancelAllActions();

        std::pair<TerminalType, sf::Vector2f> TerminalClick(const sf::FloatRect bounds, const int rot, const sf::Vector2f mouse) const;

        void UpdateHelperLines(sf::Vector2i closest);

        std::list<std::shared_ptr<GUIWire>>::const_iterator WireClick(sf::Vector2f mouse) const;

        void LoadCircuit(std::string &file);
        void SaveCircuit(std::string &file);
    
    private:
        Circuit circuit_;
        int resistors_ = 0;
        int inductors_ = 0;
        int capacitors_ = 0;
        int sources_ = 0;
        int nodes_ = 0;
        sf::Clock deltaClock_;
        sf::View view_ = sf::View(sf::FloatRect(0, 0, 640.f, 480.f));
        std::list<std::shared_ptr<GUIComponent>> components_;
        std::list<std::shared_ptr<GUIWire>> wires_;
        std::list<std::shared_ptr<GUIGround>> grounds_;
        sf::Vector2f oldPos_;
        bool movingView_ = false;  // is user moving/dragging view?
        GUIAction action_ = NO_ACTION;  // current action performed by user
        std::shared_ptr<GUIComponent> movingComponent_ = nullptr;  // pointer to component being moved
        std::shared_ptr<GUIComponent> addingComponent_ = nullptr;  // pointer to component being added
        std::shared_ptr<GUIComponent> editingComponent_ = nullptr;  // pointer to component being edited
        std::shared_ptr<GUIGround> addingGround_ = nullptr;
        std::shared_ptr<GUIWire> addingWire_ = nullptr;  // pointer to wire being added
        float zoom_ = 1;  // current zoom of view
        sf::Cursor cursor_;
        sf::VertexArray helper_lines_ = sf::VertexArray(sf::Lines, 4);
        imgui_addons::ImGuiFileBrowser file_dialog_;
        sf::VertexArray lines;
        float popup_value_;
};