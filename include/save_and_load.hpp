#pragma once

#include <string>

#include "circuit.hpp"

void SaveNetList(Circuit& circuit, const std::string& file_name);
Circuit LoadNetList(const std::string& file_name);