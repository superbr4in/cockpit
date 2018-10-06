#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <string>

class cockpit
{
    std::chrono::milliseconds const update_interval_;
    unsigned short const n_ignored_lines_;

    std::function<std::string()> const update_function_;

    std::future<void> update_future_;
    bool update_status_;

public:

    cockpit(
        std::chrono::milliseconds::rep ms_update_interval,
        unsigned short n_ignored_lines,
        std::function<std::string()> const& update_function);

    void start();
    void stop();

private:

    void update() const;
};
