#include <future>
#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/auxiliary.h>
#include <cockpit/command_monitor.h>
#include <sys/ioctl.h>
#include <unistd.h>

void get_terminal_size(int& lines, int& columns)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    lines = size.ws_row;
    columns = size.ws_col;
}

void set_cursor_pos(int line, int column)
{
    std::cout << "\x1B[" << line << ";" << column << "H" << std::flush;
}

command_monitor::command_monitor(std::string const& command, int ms_interval)
    : command_(command), ms_interval_(ms_interval), update_status_(false) { }

void command_monitor::start()
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
                update();

                std::this_thread::sleep_for(std::chrono::milliseconds(ms_interval_));
            }
        });
}
void command_monitor::stop()
{
    // Cancel a running update loop
    update_status_ = false;

    update_future_.wait();
}

void command_monitor::update() const
{
    // Get available space
    int n_lines, n_columns;
    get_terminal_size(n_lines, n_columns);

    // Leave the bottom line untouched
    n_lines -= 1;

    // Inquire the command output
    std::wstringstream wss_output_command(execute_command(command_));

    for (auto line = 1; line <= n_lines; ++line)
    {
        set_cursor_pos(line, 1);

        // Clear the current line
        std::cout << std::string(n_columns, ' ') << '\r';

        // Print the current line of the command output (if there is one)
        std::wstring line_output_command;
        if (std::getline(wss_output_command, line_output_command, L'\n'))
            std::wcout << line_output_command;

        std::cout << std::flush;
    }
}
