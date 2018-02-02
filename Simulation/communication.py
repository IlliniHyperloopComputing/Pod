import os, sys
from struct import pack

# Used to pull data from the pod
def pull(pipe, data):
	bs = pipe.read(2)
	if bs is None:
		return
	elif len(bs) != 2:
		print("len: ", bs)
		print("ERROR INPUT MALFORMED")
		print("TOO LAZY TO DEAL WITH EDGE CASE")
		return

	if bs[0] == 0:
		data["pstate"] = bs[1]
	elif bs[0] == 1:
		data["motor_enable"] = bs[1]
	elif bs[0] == 2:
		data["motor_throttle"] = bs[1]
	elif bs[0] == 3:
		data["brake_enable"] = bs[1]
	elif bs[0] == 4:
		data["brake_value"] = bs[1]

# Used to push data to the pod through the following pipe
def push(pipe, data, replace=None):
	repl = False
	if replace is not None:
		repl = True
	
	pipe.write(bytes([0]))
	if repl and replace.get("dist", None) is not None:
		pipe.write(pack('d', replace["dist"]))
	else:
		pipe.write(pack('d', data["dist"]))
	pipe.write(bytes('\n', 'UTF-8'))
	
	pipe.write(bytes([1]))
	if repl and replace.get("vel", None) is not None:
		pipe.write(pack('d', replace["vel"]))
	else:
		pipe.write(pack('d', data["vel"]))
	pipe.write(bytes("\n", 'UTF-8'))
	
	pipe.write(bytes([2]))
	if repl and replace.get("accel", None) is not None:
		pipe.write(pack('d', replace["accel"]))
	else:
		pipe.write(pack('d', data["accel"]))
	pipe.write(bytes("\n", 'UTF-8'))
