import os

def execute(command):
    return os.popen(command).read()
