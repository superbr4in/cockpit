#pragma once

#include <future>
#include <string>

class command_monitor
{
    std::string command_;

    int ms_interval_;

    std::future<void> update_future_;
    bool update_status_;

public:

    command_monitor(std::string const& command, int ms_interval);

    void start();
    void stop();

private:

    void update() const;
};
