import sys
import time
from parser import parseFromFile 
import os

delay = .05
ts = os.get_terminal_size()

numbars = ts.columns - 40

data = {
"dist":0.0,
"track_len":1400.0,
"vel":0.0,
"accel":1,
"deltaT":0.1,
"epoch": 0.0,
"drag":0.0,
"delta_drag":0.0,
"fric": 0.0,
"delta_fric":0.0
}
'''
dist = 0.0
track_len = 1400.0
vel = 0.0
accel = 1
deltaT = 0.1
epoch = 0.0
drag = 0.0
delta_drag = 0.0
fric = 0.0
delta_fric = 0.0
'''
print("Simulation")

def sign(a):
	if (a>0): return 1
	if (a==0): return 0
	return -1

def update():#(dist, vel, accel):
	global data
	# handle drag
	# accel due to drag
	
	# handle kinematics
	data["dist"] = data["dist"] + data["vel"]*data["deltaT"] + ((.5 * data["accel"]) * data["deltaT"]**2)
	data["vel"] = data["vel"] + data["accel"]*data["deltaT"]
	
	# handle drag/fric forces
	# follows this equation
	# Fdrag = ((1/2)*C*p*A) * Velocity**2
	# drag referes to the constant in this equation divided by weight of the pod
	data["accel_drag"] = data["drag"] * data["vel"]**2
	if (data["vel"] != 0 and abs(data["vel"] - sign(data["vel"])*data["accel_drag"]*data["deltaT"]) > 0):
		data["vel"] = data["vel"] - sign(data["vel"])*data["accel_drag"]*data["deltaT"]
		data["drag"] += data["delta_drag"]
		pass
	else:
		# drag will slow down pod to stop
		data["vel"] = 0
	# friction force
	# has equation Ffric = mu * Normal = fric*massOfPod
	if ( data["vel"] != 0 and abs(data["vel"] - sign(data["vel"])*data["fric"]*data["deltaT"]) > 0):
		data["vel"] = data["vel"] - sign(data["vel"])*data["fric"]*data["deltaT"]
		data["fric"] += data["delta_fric"]
		pass
	else:
		# fric will slow down pod to stop
		data["vel"] = 0
	
	if (data["dist"] > data["track_len"]):
		data["dist"] = data["track_len"]
		data["vel"] = 0
		data["accel"] = 0

	return (data["dist"], data["vel"])

def executeStatement(statement):
	#global dist, vel, accel, deltaT, drag, delta_drag, fric, delta_fric
	global data
	varname = statement[0]
	value = float(statement[1])
	
	if (varname == "a" or varname == "%v"):
		data["accel"] = value
	elif (varname == "v" or varname == "%x"):
		data["vel"] = value
	elif (varname == "x"):
		data["dist"] = value
	elif (varname == "drag"):
		data["drag"] = value
	elif (varname == "%drag"):
		data["delta_drag"] = value
	elif (varname == "fric"):
		data["fric"] = value
	elif (varname == "%fric"):
		data["delta_fric"] = value	

def executeBlock(block):
	global data
	stat = block['stat']
	for s in stat:
		s = s.split('=')
		
		if len(s) == 2:
			executeStatement(s)
		elif (s[0][0] == '?'):
			# print everything after flag
			print(s[0][1:])
'''
Handles the struct of instructions
Should a block be executed
'''
startTime = 0.0
nextBlock = 0
def handleStruct(struct, newTime):
	global nextBlock
	global data
	if (struct is None):
		return
	
	if (nextBlock >= len(struct)):
		#print("TEST2 STUFF\n", nextBlock)
		return
	
	block = struct[nextBlock]
	condl = block['cond'].split('==')
	# must have variable be time
	if (condl[0].strip() == "t"):
		# check if time has crossed over
		#print(newTime - float(condl[1]))
		if ( float(condl[1]) - data["epoch"] < 0):
			executeBlock(block)
			nextBlock += 1

def printStatus():
	global data
		
	vel = data["vel"]
	accel = data["accel"]
	dist = data["dist"]
	drag = data["drag"]
	fric = data["fric"]
	track_len = data["track_len"]
	epoch = data["epoch"]

	drawBars = int(((dist)/track_len)*numbars)
	bar = "="*drawBars
	spaces = ""
	if numbars - drawBars > 0:
		spaces = "_"*(numbars-drawBars)
	accel_drag = sign(vel) * drag * vel**2 if abs(vel) >= .001 else 0
	accel_fric = sign(vel) * fric if abs(vel) >= .001 else 0
	#sys.stdout.write('%s|%s> <d:%.2f v:%.2f a:%.2f t:%d>\r' % (bar, spaces, dist, vel if abs(vel) >= 0.001 else 0, accel - accel_fric - accel_drag, epoch))
	sys.stdout.write('%s|%s> <d:%.2f v:%.2f a:%.2f t:%d>\r' % (bar, spaces, dist, vel if abs(vel) >= 0.001 else 0, accel - accel_fric - accel_drag, epoch))
	sys.stdout.flush()
	#print(epoch, dist, vel, accel, "\r")

struct = None
if __name__ == "__main__":
	if (len(sys.argv) > 2):
		print("Include a single argument pointing to a .sym file")
	elif len(sys.argv) == 2:
		struct = parseFromFile(sys.argv[1])
		#print(json.dumps(struct, indent=4))
	#epoch = data["epoch"]

	startTime = time.time()
	printStatus()
	oldTime = time.time()
	while data["epoch"] < 200:
		newTime = time.time()
		handleStruct(struct, data["epoch"])
		deltaT = newTime-oldTime
		data["deltaT"] = deltaT
		oldTime = time.time()
		update()
		data["epoch"] = data["epoch"] + deltaT
		printStatus()
	print()
