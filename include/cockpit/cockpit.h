#pragma once

#include <functional>
#include <future>
#include <string>

class cockpit
{
    std::function<std::wstring()> update_function_;
    int update_interval_ms_;

    std::future<void> update_future_;
    bool update_status_;

public:

    cockpit(std::function<std::wstring()> update_function, int update_interval_ms);

    void start();
    void stop();

private:

    static void print(std::wstring const& text);
};
