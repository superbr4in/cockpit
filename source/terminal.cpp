#include <array>
#include <iostream>
#include <memory>
#include <string>

void terminal_set_cursor_position(unsigned short line, unsigned short column)
{
    std::cout << "\x1B[" << line << ";" << column << "H";
}

void terminal_clear_line()
{
    std::cout << "\x1B[2K";
}

std::string pipe_command_output(std::string command)
{
    std::array<char, 128> buffer;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    if (pipe == nullptr)
        throw std::runtime_error("Invalid command \"" + command + "\"");

    std::string output;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            output += buffer.data();
    }

    return output;
}
