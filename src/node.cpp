#include "node.hpp"


Node::Node(const std::string &name)
    : name_(name) { }

const std::string& Node::GetName() const {
    return name_;
}