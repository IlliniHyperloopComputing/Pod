from subprocess import Popen, PIPE
import sys
import getopt

def main():
	argv = sys.argv[1:]
	try:
		optlist, args = getopt.getopt(argv, 'p:h')
	except getopt.GetoptError as err:
		usage()
		sys.exit(2)
	print("optlist", optlist)
	print("args   ",args)

	if (len(optlist) == 0):
		usage()
		sys.exit(0)
	
	for o, a in optlist:
		if o in ("-h","-help"):
			usage()
			sys.exit()

def usage():
	print("SIMULATOR HYPERLOOP 2018™")
	print("-p <location>: path to the pod executable")
	print("-h: display usage (this)")
	print("")

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
