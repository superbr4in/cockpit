#pragma once

#include <functional>
#include <future>
#include <string>

class cockpit
{
    int const ms_update_interval_;
    int const n_ignored_lines_;

    std::function<std::wstring()> update_function_;

    std::future<void> update_future_;
    bool update_status_;

public:

    cockpit(
        int ms_update_interval,
        int n_ignored_lines,
        std::function<std::wstring()> const& update_function = []{ return std::wstring { }; });

    void set_update_function(std::function<std::wstring()> const& update_function);

    void start();
    void stop();

private:

    void update() const;
};
