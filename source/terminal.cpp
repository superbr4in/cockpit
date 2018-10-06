#include <iostream>

void terminal_set_cursor_position(unsigned short line, unsigned short column)
{
    std::cout << "\x1B[" << line << ";" << column << "H";
}

void terminal_clear_line()
{
    std::cout << "\x1B[2K";
}
