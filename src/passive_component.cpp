#include "passive_component.hpp"


PassiveComponent::PassiveComponent(const std::string &name,
                                   std::shared_ptr<Node> input,
                                   std::shared_ptr<Node> output)
    : Component(name, input, output) { }


ComponentClass PassiveComponent::GetClass() const {
    return PASSIVE;
};