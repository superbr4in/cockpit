import os, sys, termios, tty

def execute_command(command):
    return os.popen(command).read()
