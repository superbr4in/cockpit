#include <iostream>

#include <cockpit/auxiliary.h>
#include <cockpit/command_monitor.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }

    initialize_auxiliary();

    command_monitor monitor(argv[1], 1000);
    monitor.start();

    std::cin.get();

    monitor.stop();
}
