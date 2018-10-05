#pragma once

#include <functional>
#include <future>
#include <string>

class cockpit
{
    int update_interval_ms_;
    std::function<std::wstring()> update_function_;

    std::future<void> update_future_;
    bool update_status_;

public:

    cockpit(
        int update_interval_ms,
        std::function<std::wstring()> const& update_function = []{ return std::wstring { }; });

    void set_update_function(std::function<std::wstring()> const& update_function);

    void start();
    void stop();

private:

    static void print(std::wstring const& text);
};
