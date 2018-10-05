#include <sys/ioctl.h>
#include <unistd.h>

void get_terminal_size(unsigned short const* n_lines, unsigned short const* n_columns)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    n_lines = &size.ws_row;
    n_columns = &size.ws_col;
}
