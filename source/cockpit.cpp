#include <iostream>
#include <libgen.h>
#include <sstream>
#include <string>

#include <boa/boa.h>

std::wstring execute_command(std::string const& command)
{
    std::stringstream ss_file_location;
    ss_file_location << dirname(const_cast<char*>(std::string(__FILE__).c_str())) << "/auxiliary.py";

    return boa::python_file(ss_file_location.str()).call_method<std::wstring>(__FUNCTION__, command);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }

    std::string const command = argv[1];
    
    // Clear the screen
    system("tput reset");
    
    std::wcout << execute_command(command) << std::endl;

    return 0;
}
