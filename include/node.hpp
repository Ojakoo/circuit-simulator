#pragma once

#include <string>


class Node {
    public:
        Node(const std::string& name);

        const std::string& GetName() const;

    private:
        std::string name_;
};