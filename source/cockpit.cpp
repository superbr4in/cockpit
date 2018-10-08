#include <future>
#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/cockpit.h>
#include <simple-terminal/terminal.h>

cockpit::cockpit(
    std::chrono::milliseconds::rep const ms_update_interval,
    unsigned short const n_ignored_lines,
    std::function<std::string()> const& update_function) :
        update_interval_(std::chrono::milliseconds(ms_update_interval)),
        n_ignored_lines_(n_ignored_lines),
        update_function_(update_function),
        update_status_(false) { }

void cockpit::start()
{
    // Ignore multiple start attempts
    if (update_status_)
        return;

    // Start a new asynchronous update loop
    update_status_ = true;
    update_future_ = std::async(std::launch::async,
        [this]()
        {
            // Set maximum cancellation delay to 50ms
            auto const sleep_duration_part = std::chrono::milliseconds(50);

            // Update at least once
            do
            {
                auto const start_time = std::chrono::steady_clock::now();

                update();

                // Calculate the additional sleep duration
                auto const update_duration = std::chrono::steady_clock::now() - start_time;
                auto const sleep_duration = update_interval_ - update_duration;

                // Sleep with the option for cancellation
                std::this_thread::sleep_for(sleep_duration % sleep_duration_part);
                for (auto i = 0; i < sleep_duration / sleep_duration_part && update_status_; ++i)
                    std::this_thread::sleep_for(sleep_duration_part);
            }
            while (update_status_);
        });
}
void cockpit::stop()
{
    // Stop a running update loop
    update_status_ = false;
    update_future_.wait();
}

void cockpit::update() const
{
    auto n_lines = term::get_size().first;

    // Leave the specified number of lines untouched
    if (n_ignored_lines_ < n_lines)
        n_lines -= n_ignored_lines_;
    else n_lines = 0;

    // Inquire the function output
    std::stringstream ss_output(update_function_());

    for (unsigned short line = 1; line <= n_lines; ++line)
    {
        term::set_cursor(line, 1);

        // Clear the current line
        term::clear_line();

        // Print the current line of the function output (if there is another)
        std::string output_line;
        if (std::getline(ss_output, output_line, '\n'))
            std::cout << output_line;
    }

    // Reset cursor
    term::set_cursor(1, 1);

    std::cout << std::flush;
}
