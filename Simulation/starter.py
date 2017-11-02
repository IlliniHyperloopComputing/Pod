from subprocess import Popen, PIPE
import sys

args = sys.argv
if len(args) == 1:
	args = ("python3", "simulation.py")
else:
	# remove name of python file
	args = args[1:]
print(args, type(args))

cproc = Popen(args, stdout=PIPE, stdin=PIPE);
print("Started new program PID =", cproc.pid)

while True:
	line = cproc.stdout.read(1)
	line = line.decode("utf-8")
	if line == '' and cproc.poll() != None:
		break
	if line != '':
		sys.stdout.write(line)

print("done with starter.py")
