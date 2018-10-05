import subprocess

def pipe_command(command):
    with subprocess.Popen(command.split(), stdout=subprocess.PIPE) as p:
        return p.stdout.read().decode('utf-8')
