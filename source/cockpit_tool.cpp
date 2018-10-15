#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cockpit/cockpit.h>
#include <termaux/termaux.h>

std::string pipe_command_output(std::string command)
{
    std::array<char, 0x80> buffer;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    std::string output;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            output += buffer.data();
    }

    return output;
}

int main(int argc, char const* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    std::string const command(argv[1]);

    // Get current terminal size
    auto n_lines = tmx::get_lines();

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
        key = tmx::read_key();

        // Get current terminal size
        n_lines = tmx::get_lines();

        // Handle scrolling
        auto has_scrolled = true;
        switch (key)
        {
        case tmx::KEY_ARROW_UP:
            --scroll;
            break;
        case tmx::KEY_ARROW_DOWN:
            ++scroll;
            break;
        case tmx::KEY_PAGE_UP:
            scroll -= n_lines;
            break;
        case tmx::KEY_PAGE_DOWN:
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
    while (key != tmx::KEY_ESCAPE);

    // Stop running cockpit instance
    cockpit.stop();

    // Move cursor to the bottom line
    tmx::set_cursor(n_lines);

    return 0;
}
