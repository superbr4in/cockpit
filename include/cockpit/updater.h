#pragma once

#include <string>

class updater
{
    std::string command_;

    int ms_interval_;

public:

    updater(std::string const& command, int ms_interval);

    void run() const;
};
