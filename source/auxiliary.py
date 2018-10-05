import os

def pipe_command(command):
    return os.popen(command).read()
