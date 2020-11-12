#include "active_component.hpp"


ActiveComponent::ActiveComponent(const std::string &name,
                                 std::shared_ptr<Node> input,
                                 std::shared_ptr<Node> output)
    : Component(name, input, output) { }


ComponentClass ActiveComponent::GetClass() const {
    return ACTIVE;
};