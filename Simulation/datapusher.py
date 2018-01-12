import os, sys
from struct import pack

# Used to push data to the pod through the following pipe
def push(pipe, data, replace=None):
	repl = False
	if replace is not None:
		repl = True
	
	pipe.write(bytes("0", 'UTF-8'))
	if repl and replace.get("dist", None) is not None:
		pipe.write(pack('d', replace["dist"]))
	else:
		pipe.write(pack('d', data["dist"]))
	pipe.write(bytes('\n', 'UTF-8'))
	'''
	pipe.write(bytes("1", 'UTF-8'))
	if repl and replace.get("vel", None) is not None:
		pipe.write(pack('d', replace["vel"]))
	else:
		pipe.write(pack('d', data["vel"]))
	pipe.write(bytes("\n", 'UTF-8'))
	
	pipe.write(bytes("2", 'UTF-8'))
	if repl and replace.get("accel", None) is not None:
		pipe.write(pack('d', replace["accel"]))
	else:
		pipe.write(pack('d', data["accel"]))
	pipe.write(bytes("\n", 'UTF-8'))
	'''
'''
if __name__ == "__main__":
	r, w = os.pipe()
	push(os.fdopen(w,'w'))
	r = os.fdopen(r)
	print(r.read())
'''
