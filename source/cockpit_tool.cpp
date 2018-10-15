#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boa/boa.h>
#include <cockpit/cockpit.h>
#include <simple-terminal/terminal.h>

auto constexpr KEY_ESCAPE = 0x1B;

auto constexpr KEY_ARROW_UP   = 0x415B1B;
auto constexpr KEY_ARROW_DOWN = 0x425B1B;
auto constexpr KEY_PAGE_UP    = 0x7E355B1B;
auto constexpr KEY_PAGE_DOWN  = 0x7E365B1B;

std::string pipe_command_output(std::string command)
{
    std::array<char, 128> buffer;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    std::string output;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            output += buffer.data();
    }

    return output;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    std::string const command(argv[1]);

    // Get auxiliary python file from binary directory
    boa::python_file const py_aux(std::string(argv[0]) + "_aux.py");

    // Get current terminal size
    auto n_lines = term::get_size().first;

    // Create space not to overwrite existing output
    std::cout << std::string(n_lines, '\n') << std::flush;

    // Initialize scroll value
    auto scroll = 0;

    // Start new cockpit instance
    cockpit cockpit(1000, 1,
        [command, &scroll]
        {
            // Pass command output string
            return std::make_pair(pipe_command_output(command), &scroll);
        });
    cockpit.start();

    // Loop through user input characters
    uint32_t key = 0;
    do
    {
        // Wait for a pressed key
        key = py_aux.call_function<uint32_t>("read_key");

        // Get current terminal size
        n_lines = term::get_size().first; 

        // Handle scrolling
        auto has_scrolled = true;
        switch (key)
        {
        case KEY_ARROW_UP:
            --scroll;
            break;
        case KEY_ARROW_DOWN:
            ++scroll;
            break;
        case KEY_PAGE_UP:
            scroll -= n_lines;
            break;
        case KEY_PAGE_DOWN:
            scroll += n_lines;
            break;
        default:
            has_scrolled = false;
            break;
        }

        // Update once instantly if scrolled
        if (has_scrolled)
            cockpit.fire();
    }
    // Exit if ESC is pressed
    while (key != KEY_ESCAPE);

    // Stop running cockpit instance
    cockpit.stop();

    // Move cursor to the bottom line
    term::set_cursor(n_lines, 1);

    return 0;
}
