#include <iostream>
#include <sstream>
#include <thread>

#include <cockpit/auxiliary.h>
#include <cockpit/updater.h>
#include <sys/ioctl.h>
#include <unistd.h>

void terminal_clear()
{
    system("tput reset");
}

void terminal_size(int& lines, int& columns)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    lines = size.ws_row;
    columns = size.ws_col;
}

updater::updater(std::string const& command, int ms_interval)
    : command_(command), ms_interval_(ms_interval) { }

void updater::run() const
{
    while (true)
    {
        int lines, columns;
        terminal_size(lines, columns);

        std::wstringstream ss_output(execute_command(command_));
        std::wstringstream ss_output_trimmed;
        std::wstring output_buffer;
        for (auto i = 0; i < lines - 2 && std::getline(ss_output, output_buffer, L'\n'); ++i)
            ss_output_trimmed << output_buffer << std::endl;

        terminal_clear();
        std::wcout << ss_output_trimmed.str() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(ms_interval_));
    }
}
