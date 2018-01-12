import os, sys

# Used to push data to the pod through the following pipe
def push(pipe, data, replace=None):
	repl = False
	if replace is not None:
		repl = True
	msg = ""
	if repl and replace.get("dist", None) is not None:
		msg = "0" + str(replace["dist"])+"\n"
	else:
		msg = "0" + str(data["dist"]) + "\n"
	pipe.write(bytes(msg, 'UTF-8'))
	
	if repl and replace.get("vel", None) is not None:
		msg = "0" + str(replace["vel"])+"\n"
	else:
		msg = "0" + str(data["vel"]) + "\n"
	pipe.write(bytes(msg, 'UTF-8'))

	if repl and replace.get("accel", None) is not None:
		msg = "0" + str(replace["accel"])+"\n"
	else:
		msg = "0" + str(data["accel"]) + "\n"
	pipe.write(bytes(msg, 'UTF-8'))
	
'''
if __name__ == "__main__":
	r, w = os.pipe()
	push(os.fdopen(w,'w'))
	r = os.fdopen(r)
	print(r.read())
'''
