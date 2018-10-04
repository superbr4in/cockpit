#include <iostream>

#include <cockpit/cockpit.h>
#include <cockpit_tool/auxiliary.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }

    initialize_auxiliary();

    cockpit cockpit([argv]() { return execute_command(argv[1]); }, 1000);
    cockpit.start();

    std::cin.get();

    cockpit.stop();
}
