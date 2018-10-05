#include <future>
#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/cockpit.h>
#include <cockpit/terminal.h>

void set_cursor_position(int line, int column)
{
    std::cout << "\x1B[" << line << ";" << column << "H" << std::flush;
}
void clear_line()
{
    std::cout << "\x1B[2K" << std::flush;
}

cockpit::cockpit(int update_interval_ms, std::function<std::wstring()> const& update_function)
    : update_interval_ms_(update_interval_ms), update_function_(update_function), update_status_(false) { }

void cockpit::set_update_function(std::function<std::wstring()> const& update_function)
{
    update_function_ = update_function;
}

void cockpit::start()
{
    if (update_status_)
        return;

    update_status_ = true;

    // Start an update loop
    update_future_ = std::async(std::launch::async,
        [this]()
        {
            system("tput reset");

            while (update_status_)
            {
                auto const start_time = std::chrono::steady_clock::now();

                print(update_function_());

                auto const duration = std::chrono::steady_clock::now() - start_time;
                std::this_thread::sleep_for(std::chrono::milliseconds(update_interval_ms_) - duration);
            }
        });
}
void cockpit::stop()
{
    // Cancel a running update loop
    update_status_ = false;

    update_future_.wait();
}

void cockpit::print(std::wstring const& text)
{
    // Get available space
    unsigned short n_lines, n_columns;
    get_terminal_size(&n_lines, &n_columns);

    // Leave the bottom line untouched
    n_lines -= 1;

    // Inquire the command output
    std::wstringstream wss_output(text);

    for (auto line = 1; line <= n_lines; ++line)
    {
        set_cursor_position(line, 1);

        // Clear the current line
        clear_line();

        // Print the current line of the command output (if there is one)
        std::wstring line_output_command;
        if (std::getline(wss_output, line_output_command, L'\n'))
            std::wcout << line_output_command;

        std::cout << std::flush;
    }
}
