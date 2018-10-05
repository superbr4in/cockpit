#pragma once

#include <string>

void initialize_python();

std::wstring py_execute_command(std::string const& command);
