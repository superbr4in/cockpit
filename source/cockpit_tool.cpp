#include <iostream>
#include <string>

#include <boa/boa.h>
#include <cockpit/cockpit.h>
#include <cockpit/terminal.h>

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

    // Get auxiliary python file from binary directory
    boa::python_file const py_aux(std::string(argv[0]) + "_aux.py");

    std::string const command = argv[1];

    unsigned short n_lines, n_columns;
    terminal_get_size(&n_lines, &n_columns);

    // Create space not to overwrite existing output
    std::cout << std::string(n_lines, '\n') << std::flush;

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
    
    py_aux.call_function<std::string>("read_character");

    cockpit.stop();

    // Clear exit text
    terminal_set_cursor_position(n_lines, 1);
    terminal_clear_line();

    std::cout << std::flush;

    return 0;
}
