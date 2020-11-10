#pragma once

#include <string>


// There are two types of nodes: normal and ground
enum NodeType {
    NORMAL,
    GROUND
};


class Node {

    public:
        Node(const std::string& name, NodeType type = NORMAL);

        const std::string& GetName() const;

        const NodeType GetType() const;

        void SetNodeType(NodeType type);

    private:
        std::string name_;
        NodeType type_;
};