#include <array>
#include <iostream>
#include <memory>
#include <string>

std::string exec(std::string const& command)
{
    std::array<char, 128> buffer;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
        throw std::runtime_error("popen failed");

    std::string result;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }

    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // TODO: Print help
        return 0;
    }
    
    // Clear the screen
    system("tput reset");

    auto const result = exec(argv[1]);

    std::cout << "\033[1;31m" << "Execution result:" << "\033[0m" << std::endl;
    std::cout << result << std::endl;

    return 0;
}

