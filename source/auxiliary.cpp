#include <memory>
#include <string>

#include <boa/boa.h>

std::unique_ptr<boa::python_file> python_file;

void initialize_python()
{
    // Look for a file with the same name and a .py extension
    std::string const file_path_cpp(__FILE__);
    std::string const file_path_py = file_path_cpp.substr(0, file_path_cpp.find_last_of('.')) + ".py";

    python_file = std::make_unique<boa::python_file>(file_path_py.c_str());
}

std::string py_read_character()
{
    return python_file->call_function<std::string>("read_character");
}
