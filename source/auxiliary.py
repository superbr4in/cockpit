import os, sys, termios, tty

def execute_command(command):
    return os.popen(command).read()

def read_character():
    old_settings = termios.tcgetattr(sys.stdin)
    try:
        tty.setraw(sys.stdin)
        c = sys.stdin.read(1)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
    return c
