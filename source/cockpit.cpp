#include <cockpit/auxiliary.h>
#include <cockpit/updater.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }

    initialize_auxiliary();

    updater const u(argv[1], 1000);
    u.run();
}
