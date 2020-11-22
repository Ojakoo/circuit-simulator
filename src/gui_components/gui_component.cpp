#include "gui_components/gui_component.hpp"


GUIComponent::GUIComponent(const std::string& texture, std::shared_ptr<Component> component)
    : sf::Sprite() {
        // set texture
        tx_ = sf::Texture();
        tx_.loadFromFile(texture);
        setTexture(tx_);

        // set component
        component_ = component;
     }

const std::string GUIComponent::GetName() const {
    return component_->GetName();
}

const ComponentType GUIComponent::GetType() const {
    return component_->GetType();
}

const std::shared_ptr<Component> GUIComponent::GetComponent() const {
    return component_;
}