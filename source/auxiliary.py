import subprocess

def to_unicode(string):
    return string.decode('utf-8')

def pipe_command(command):
    with subprocess.Popen(command.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE) as p:
        stdout = p.stdout.read()
        if len(stdout) > 0:
            return to_unicode(stdout)
        return to_unicode(p.stderr.read())
