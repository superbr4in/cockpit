#include <iostream>
#include <libgen.h>
#include <sstream>
#include <string>

#include <boa/boa.h>

constexpr auto py_file_name = "auxiliary.py";
constexpr auto py_method_name = "execute_command";

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }
    
    // Clear the screen
    system("tput reset");

    std::stringstream ss_file_path;
    ss_file_path << dirname(const_cast<char*>(std::string(__FILE__).c_str())) << '/' << py_file_name;

    std::wcout
        << py_call_method(ss_file_path.str(), py_method_name, std::string(argv[1]))
        << std::endl;

    return 0;
}
