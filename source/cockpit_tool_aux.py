import os, sys, termios, tty

def read_key():
    prev_settings = termios.tcgetattr(sys.stdin)
    try:
        tty.setcbreak(sys.stdin)
        code = os.read(sys.stdin.fileno(), 4)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, prev_settings)
    key = 0
    for i in range(len(code)):
        key += code[i] << (len(code) - i - 1) * 8
    return key
