#pragma once

extern "C"
{
    void terminal_get_size(unsigned short* n_lines, unsigned short* n_columns);
}

void terminal_set_cursor_position(unsigned short line, unsigned short column);

void terminal_clear_line();
