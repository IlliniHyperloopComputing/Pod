from subprocess import Popen, PIPE
from parser import parseFromFile 
from simulation import printStatus
from simulation import handleStruct, update
from communication import push, pull
from fcntl import fcntl, F_GETFL, F_SETFL
import time
import sys
import getopt
import os
from os import O_NONBLOCK

ts = os.get_terminal_size()
numbars = ts.columns - 40
data = {
"pstate":0,
"motor_enable":0,
"motor_throttle":0,
"brake_enable":0,
"brake_value":0,

"dist":0.0,
"track_len":1400.0,
"vel":0.0,
"accel":0,
"deltaT":0.1,
"epoch": 0.0,
"drag":0.0,
"delta_drag":0.0,
"fric": 0.0,
"delta_fric":0.0
}

replace = {
}

def main():
	argv = sys.argv[1:]
	try:
		optlist, args = getopt.getopt(argv, 'e:hs:')
	except getopt.GetoptError as err:
		print(err)
		usage()
		sys.exit(2)
	#print("optlist", optlist)
	#print("args   ",args)

	if (len(optlist) == 0):
		usage()
		sys.exit(0)
	
	execpath = None
	fname = None

	for o, a in optlist:
		if o in ("-h","-help"):
			usage()
			sys.exit()
		elif o in ("-e", "-exec", "-executable"):
			execpath = a;
		elif o in ("-s", "-script"):
			fname = a

	if execpath is None:
		print("Please specify a path to pod executable")
		sys.exit()
	
	cproc = Popen((execpath,), stdout=PIPE, stdin=PIPE)
	flags = fcntl(cproc.stdin, F_GETFL)
	fcntl(cproc.stdin, F_SETFL, flags|O_NONBLOCK)
	datin = cproc.stdin
	flags = fcntl(cproc.stdout, F_GETFL)
	fcntl(cproc.stdout, F_SETFL, flags|O_NONBLOCK)
	datout = cproc.stdout
	
	time.sleep(1)
	
	if fname is None:
		struct = None
	else:
		struct = parseFromFile(fname)

	startTime = time.time()
	printStatus(data, numbars)
	oldTime = time.time()
	counter = 0
	while data["epoch"] < 200:
		newTime = time.time()
		handleStruct(struct, data, replace)
		deltaT = newTime-oldTime
		data["deltaT"] = deltaT
		oldTime = time.time()
		update(data)
		if counter is 0:
			push(datin, data, replace)
		counter = (counter + 1)%200
		pull(datout, data)
		data["epoch"] = data["epoch"] + deltaT
		printStatus(data, numbars)
	print()

def usage():
	print("SIMULATOR HYPERLOOP 2018™")
	print("-e <location>: path to the pod executable")
	print("-s <name>: file name for script file")
	print("-h: display usage (this)")

if __name__ == "__main__":
	main()

'''
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
'''
