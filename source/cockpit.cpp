#include <future>
#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/cockpit.h>
#include <cockpit/terminal.h>

cockpit::cockpit(
    int ms_update_interval,
    int n_ignored_lines,
    std::function<std::wstring()> const& update_function) :
        ms_update_interval_(ms_update_interval),
        n_ignored_lines_(n_ignored_lines),
        update_function_(update_function),
        update_status_(false) { }

void cockpit::set_update_function(std::function<std::wstring()> const& update_function)
{
    update_function_ = update_function;
}

void cockpit::start()
{
    // Ignore multiple start attempts
    if (update_status_)
        return;

    update_status_ = true;

    // Start a new update loop
    update_future_ = std::async(std::launch::async,
        [this]()
        {
            unsigned short n_lines, n_columns;
            terminal_get_size(&n_lines, &n_columns);

            // Create space not to overwrite existing output
            std::cout << std::string(n_lines, '\n') << std::flush;

            while (update_status_)
            {
                auto const start_time = std::chrono::steady_clock::now();

                update();

                // Calculate the additional sleep duration
                auto const update_duration = std::chrono::steady_clock::now() - start_time;
                auto const sleep_duration = std::chrono::milliseconds(ms_update_interval_) - update_duration;

                std::this_thread::sleep_for(sleep_duration);
            }
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
    unsigned short n_lines, n_columns;
    terminal_get_size(&n_lines, &n_columns);

    // Leave the specified number of bottom lines untouched
    n_lines -= n_ignored_lines_;

    // Inquire the function output
    std::wstringstream wss_output(update_function_());

    for (unsigned short line = 1; line <= n_lines; ++line)
    {
        terminal_set_cursor_position(line, 1);

        // Clear the current line
        terminal_clear_line();

        // Print the current line of the function output (if there is another)
        std::wstring output_line;
        if (std::getline(wss_output, output_line, L'\n'))
            std::wcout << output_line;

        std::cout << std::flush;
    }
}
