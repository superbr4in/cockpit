#include <algorithm>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/cockpit.h>
#include <simple-terminal/terminal.h>

cockpit::cockpit(
    std::chrono::milliseconds::rep const ms_update_interval,
    unsigned short const n_ignored_lines,
    std::function<std::pair<std::string, int*>()> const& update_function) :
        update_interval_(std::chrono::milliseconds(ms_update_interval)),
        n_ignored_lines_(n_ignored_lines),
        update_function_(update_function),
        update_(false),
        fire_(false) { }

void cockpit::start()
{
    // Ignore multiple start attempts
    if (update_)
        return;

    // Start a new asynchronous update loop
    update_ = true;
    update_future_ = std::async(std::launch::async,
        [this]()
        {
            // Set maximum interrupt delay to 50ms
            auto const sleep_duration_part = std::chrono::milliseconds(50);

            // Loop updates (at least once)
            do
            {
                // Reset an instant update request
                fire_ = false;

                auto const start_time = std::chrono::steady_clock::now();

                update();

                // Calculate the additional sleep duration
                auto const update_duration = std::chrono::steady_clock::now() - start_time;
                auto const sleep_duration = update_interval_ - update_duration;

                // Sleep with the ability to be interrupted
                std::this_thread::sleep_for(sleep_duration % sleep_duration_part);
                for (auto i = 0; i < sleep_duration / sleep_duration_part && update_ && !fire_; ++i)
                    std::this_thread::sleep_for(sleep_duration_part);
            }
            while (update_);
        });
}
void cockpit::stop()
{
    // Stop a running update loop
    update_ = false;
    update_future_.wait();
}

void cockpit::fire()
{
    // Request an instant update
    fire_ = true;
}

void cockpit::update() const
{
    auto n_lines = term::get_size().first;

    // Leave the specified number of lines untouched
    if (n_ignored_lines_ < n_lines)
        n_lines -= n_ignored_lines_;
    else n_lines = 0;

    // Inquire the function output
    auto const [output, scroll] = update_function_();

    std::vector<std::string> output_lines;

    std::stringstream ss_output(output);
    std::string output_line;
    while (std::getline(ss_output, output_line))
        output_lines.push_back(output_line);

    if (output_lines.empty())
        output_lines.push_back({ });

    *scroll = std::clamp(*scroll, 0, static_cast<int>(output_lines.size() - 1));

    for (unsigned short line = 0; line < n_lines; ++line)
    {
        term::set_cursor(line + 1, 1);

        // Clear the current line
        term::clear_line();

        // Print the current line (if there is another)
        auto scroll_line = line + *scroll;
        if (scroll_line < output_lines.size())
            std::cout << output_lines.at(scroll_line);
    }

    // Reset cursor
    term::set_cursor(1, 1);

    std::cout << std::flush;
}
