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

    unsigned short n_lines, n_columns;
    terminal_get_size(&n_lines, &n_columns);

    // Create space not to overwrite existing output
    std::cout << std::string(n_lines, '\n') << std::flush;

    terminal_set_cursor_position(n_lines - 1, 1);
    std::cout << "Press ENTER to exit...";

    initialize_python();

    cockpit cockpit(1000, 2, [argv] { return py_execute_command(argv[1]); });
    cockpit.start();

    std::cin.get();

    cockpit.stop();

    terminal_set_cursor_position(n_lines, 1);
}
