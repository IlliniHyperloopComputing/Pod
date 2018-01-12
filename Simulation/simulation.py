import sys
import time
from parser import parseFromFile 
from parser import is_number
import os

#delay = .05
#ts = os.get_terminal_size()

#numbars = ts.columns - 40

def sign(a):
	if (a>0): return 1
	if (a==0): return 0
	return -1

def update(data):#(dist, vel, accel):
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

def executeStatement(statement, data, variables):
	varname = statement[0]
	if is_number(statement[1]):
		value = float(statement[1])
	else:
		value = nameToValue(variables, statement[1])
	
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
	else:
		print("Unknown variable", varname)

def removeReplacer(varname, replace):
	if (varname == "a" or varname == "%v"):
		del replace["accel"]
	elif (varname == "v" or varname == "%x"):
		del replace["vel"]
	elif (varname == "x"):
		del replace["dist"]
	elif (varname == "drag"):
		del replace["drag"]
	elif (varname == "%drag"):
		del replace["delta_drag"]
	elif (varname == "fric"):
		del replace["fric"]
	elif (varname == "%fric"):
		del replace["delta_fric"]

def nameToValue(data, varname):
	if (varname == "a" or varname == "%v"):
		return data["accel"]
	elif (varname == "v" or varname == "%x"):
		return data["vel"]
	elif (varname == "x"):
		return data["dist"]
	elif (varname == "drag"):
		return data["drag"]
	elif (varname == "%drag"):
		return data["delta_drag"]
	elif (varname == "fric"):
		return data["fric"]
	elif (varname == "%fric"):
		return data["delta_fric"]
	else:
		return 0

def executeBlock(block, data, replace):
	stat = block['stat']
	for s in stat:
		s = s.split('=')
		if len(s) == 2:
			temp = data
			if s[0][0] is '$':
				temp = replace
				s[0] = s[0][1:]
			executeStatement(s, temp, data)
		elif (s[0][0] == '?'):
			if s[0][1:] is "*":
				print(data)
				print("----")
				print(replace)
			else:
				# print everything after flag
				print(s[0][1:])
		elif s[0][0] is '$':
			#removes the replace field for this variable
			removeReplacer(s[0][1:], replace)
'''
Handles the struct of instructions
Should a block be executed
'''
startTime = 0.0
nextBlock = 0
def handleStruct(struct, data, replace):
	global nextBlock
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
			executeBlock(block, data, replace)
			nextBlock += 1

def printStatus(data, numbars):
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
