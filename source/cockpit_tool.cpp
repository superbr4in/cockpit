#include <iostream>

#include <cockpit/cockpit.h>
#include <cockpit/terminal.h>
#include <cockpit_tool/auxiliary.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }

    std::string const command = argv[1];

    unsigned short n_lines, n_columns;
    terminal_get_size(&n_lines, &n_columns);

    // Create space not to overwrite existing output
    std::cout << std::string(n_lines, '\n') << std::flush;

    initialize_python();

    cockpit cockpit(1000, 1,
        [command, &n_lines, &n_columns]
        {
            terminal_get_size(&n_lines, &n_columns);

            // Print exit text
            terminal_set_cursor_position(n_lines, 1);
            terminal_clear_line();
            std::cout << std::string("Press ENTER to exit...").substr(0, n_columns);

            return py_pipe_command(command);
        });
    cockpit.start();

    std::cin.get();

    cockpit.stop();

    // Clear exit text
    terminal_set_cursor_position(n_lines, 1);
    terminal_clear_line();

    std::cout << std::flush;
}
