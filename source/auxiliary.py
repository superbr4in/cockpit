import os, sys, termios, tty

def read_character():
    prev_settings = termios.tcgetattr(sys.stdin)
    try:
        tty.setcbreak(sys.stdin)
        return os.read(sys.stdin.fileno(), 4)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, prev_settings)
