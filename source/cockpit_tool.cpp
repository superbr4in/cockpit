#include <iostream>

#include <cockpit/cockpit.h>
#include <cockpit/terminal.h>
#include <cockpit_tool/auxiliary.h>
#include <cockpit_tool/terminal.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
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
            std::cout << std::string("Press any key to exit...").substr(0, n_columns);

            return pipe_command_output(command);
        });
    cockpit.start();

    py_read_character();

    cockpit.stop();

    // Clear exit text
    terminal_set_cursor_position(n_lines, 1);
    terminal_clear_line();

    std::cout << std::flush;
}
