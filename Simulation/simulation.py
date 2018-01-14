import sys
import time
from parser import parseFromFile 
from parser import is_number
from failureExcept import FailureException
import os

#delay = .05
#ts = os.get_terminal_size()

#numbars = ts.columns - 40

def sign(a):
	if (a>0): return 1
	if (a==0): return 0
	return -1

'''
"motor_enable":0, # 0 is disabled, 1 is enabled
"motor_throttle":0,
"brake_enable":0,
"brake_value":0,

"max_motor":1, # WHEN MAX THROTTLE, this is the accel the pod will feel
"max_brake":-1, # WHEN MAX BRAKE, this is the applied accel the pod will feel
'''

def update(data):#(dist, vel, accel):
	net_accel = data["accel"]
	
	# Handle pod controlls
	if data["motor_enable"] != 0:
		net_accel = net_accel + data["motor_throttle"]*data["max_motor"]

	if data["brake_enable"] != 0:
		accel_brake = data["brake_value"]*data["max_brake"] # MAX BRAKE SHOULD BE NEG
		if (data["vel"] != 0 and abs(data["vel"] - sign(data["vel"])*accel_brake*data["deltaT"])>0):
			net_accel = net_accel + sign(data["vel"])*accel_brake
			pass
		else:
			# brake will slow down pod to stop
			data["vel"] = 0
	# handle drag/fric forces
	# follows this equation
	# Fdrag = ((1/2)*C*p*A) * Velocity**2
	# drag referes to the constant in this equation divided by weight of the pod
	accel_drag = data["drag"] * data["vel"]**2
	if (data["vel"] != 0 and abs(data["vel"] - sign(data["vel"])*accel_drag*data["deltaT"]) > 0):
		# Apply accel in opposite direction
		net_accel = net_accel - sign(data["vel"])*accel_drag
		pass
	else:
		# drag will slow down pod to stop
		data["vel"] = 0
	data["drag"] += data["delta_drag"]*data["deltaT"]
	
	# friction force
	# has equation Ffric = mu * Normal = fric*massOfPod
	if (data["vel"] != 0 and abs(data["vel"] - sign(data["vel"])*data["fric"]*data["deltaT"]) > 0):
		net_accel = net_accel - sign(data["vel"])*data["fric"]
		pass
	else:
		# fric will slow down pod to stop
		data["vel"] = 0
	data["fric"] += data["delta_fric"]*data["deltaT"] # mult by deltaTime to preserve rate
	
	# handle kinematics
	data["dist"] = data["dist"] + data["vel"]*data["deltaT"] + ((.5*net_accel) * data["deltaT"]**2)
	data["vel"] = data["vel"] + net_accel*data["deltaT"]
	
	data["net_accel"] = net_accel
	return (data["dist"], data["vel"])


def executeStatementSub(statement, data, variables):
	varname = statement[0]
	if is_number(statement[1]):
		value = float(statement[1])
	else:
		value = nameToValue(variables, statement[1])
	
	if (varname == "a" or varname == "%v"):
		data["accel"] -= value
	elif (varname == "v" or varname == "%x"):
		data["vel"] -= value
	elif (varname == "x"):
		data["dist"] -= value
	elif (varname == "drag"):
		data["drag"] -= value
	elif (varname == "%drag"):
		data["delta_drag"] -= value
	elif (varname == "fric"):
		data["fric"] -= value
	elif (varname == "%fric"):
		data["delta_fric"] -= value
	elif (varname in ["motor_en"]):
		data["motor_enable"] -= value
	elif (varname in ["motor_thr"]):
		data["motor_throttle"] -= value
	elif (varname in ["brake_en"]):
		data["brake_enable"] -= value
	elif (varname in ["brake_val"]):
		data["brake_value"] -= value
	else:
		print("Unknown variable", varname)

def executeStatementAdd(statement, data, variables):
	varname = statement[0]
	if is_number(statement[1]):
		value = float(statement[1])
	else:
		value = nameToValue(variables, statement[1])
	
	if (varname == "a" or varname == "%v"):
		data["accel"] += value
	elif (varname == "v" or varname == "%x"):
		data["vel"] += value
	elif (varname == "x"):
		data["dist"] += value
	elif (varname == "drag"):
		data["drag"] += value
	elif (varname == "%drag"):
		data["delta_drag"] += value
	elif (varname == "fric"):
		data["fric"] += value
	elif (varname == "%fric"):
		data["delta_fric"] += value
	elif (varname in ["motor_en"]):
		data["motor_enable"] += value
	elif (varname in ["motor_thr"]):
		data["motor_throttle"] += value
	elif (varname in ["brake_en"]):
		data["brake_enable"] += value
	elif (varname in ["brake_val"]):
		data["brake_value"] += value
	else:
		print("Unknown variable", varname)

def executeStatementMult(statement, data, variables):
	varname = statement[0]
	if is_number(statement[1]):
		value = float(statement[1])
	else:
		value = nameToValue(variables, statement[1])
	
	if (varname == "a" or varname == "%v"):
		data["accel"] *= value
	elif (varname == "v" or varname == "%x"):
		data["vel"] *= value
	elif (varname == "x"):
		data["dist"] *= value
	elif (varname == "drag"):
		data["drag"] *= value
	elif (varname == "%drag"):
		data["delta_drag"] *= value
	elif (varname == "fric"):
		data["fric"] *= value
	elif (varname == "%fric"):
		data["delta_fric"] *= value
	elif (varname in ["motor_en"]):
		data["motor_enable"] *= value
	elif (varname in ["motor_thr"]):
		data["motor_throttle"] *= value
	elif (varname in ["brake_en"]):
		data["brake_enable"] *= value
	elif (varname in ["brake_val"]):
		data["brake_value"] *= value
	else:
		print("Unknown variable", varname)

def executeStatementSet(statement, data, variables):
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
	elif (varname in ["motor_en"]):
		data["motor_enable"] = value
	elif (varname in ["motor_thr"]):
		data["motor_throttle"] = value
	elif (varname in ["brake_en"]):
		data["brake_enable"] = value
	elif (varname in ["brake_val"]):
		data["brake_value"] = value
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
	elif (varname in ["t"]):
		return data["epoch"]
	elif (varname in ["dt"]):
		return data["deltaT"]
	elif (varname in ["track_len"]):
		return data["track_len"]
	elif (varname in ["motor_en"]):
		return data["motor_enable"]
	elif (varname in ["motor_thr"]):
		return data["motor_throttle"]
	elif (varname in ["brake_en"]):
		return data["brake_enable"]
	elif (varname in ["brake_val"]):
		return data["brake_value"]
	elif (varname in ["net_a", "na", "neta"]):
		return data["net_accel"]
	elif (varname in ["pstate"]):
		return data["pstate"]
	else:
		print("Unkown Variable", varname)
		return 0

def executeBlock(block, data, replace):
	deleteB = False
	stat = block['stat']
	for s in stat:
		typeop = '='
		if '*=' in s:
			s = s.split('*=')
			typeop = '*='
		elif '-=' in s:
			s = s.split('-=')
			typeop = '-='
		elif '+=' in s:
			s = split('+=')
			typeop = '+='
		else:
			s = s.split('=')
		
		if len(s) == 2:
			temp = data
			if s[0][0] is '$':
				temp = replace
				s[0] = s[0][1:]
			if typeop is '=':
				executeStatementSet(s, temp, data)
			elif typeop is '*=':
				print(s)
				executeStatementMult(s, temp, data)
			elif typeop is '+=':
				executeStatementAdd(s, temp, data)
			elif typeop is '-=':
				executeStatementSub(s, temp, data)
		elif (s[0][0] == '?'):
			if s[0][1:].startswith('*'):
				print(s[0][2:])
				print(data)
				print("----")
				print(replace)
			else:
				# print everything after flag
				print(s[0][1:])
		elif s[0][0] is '^':
			# ^ represents delete block, used to prevent a block returning true infinetly
			deleteB = True
		elif s[0][0] is '$':
			#removes the replace field for this variable
			removeReplacer(s[0][1:], replace)
		elif s[0][0] is '!':
			# Failure condition
			raise FailureException(s[0][1:])
	return deleteB

'''
Checks a signle cond
e.g. x==1
returns (bool, usesTime=False)
usesTime is useful inorder to make sure that the struct is removed
'''
def checkCondSingle(condstate, data):
	operator = None
	condl = None
	usesTime = False
	varval = None
	checkval = None
	for op in ['==', '>=', '<=', '<', '>', '!=']:
		condl = condstate.split(op)
		if (len(condl) > 2):
			assert False # Should never be true
		elif (len(condl) == 2):
			operator = op
			if condl[0].strip() is "t":
				usesTime = True
			varval = nameToValue(data, condl[0].strip())
			if is_number(condl[1]):
				checkval = float(condl[1])
			else:
				checkval = nameToValue(data, condl[1].strip())
			#print(varval, checkval)
			break
	if varval is None:
		print("malformed operator")
		return (False, False)
	res = False
	neg = False
	# check negs first
	if operator is "!=":
		operator = "=="
		neg = True
	elif operator is ">=":
		operator = "<"
		neg = True
	elif operator is "<=":
		operator = ">"
		neg = True
	if usesTime is True:
		# check regular operators
		if operator is "==":
			# check if this exec passed the checktime
			if checkval == 0.0:
				# time == 0 should always be true
				# since when we start executing this should return true
				res = True
			elif varval > checkval and varval - data["deltaT"] < checkval:
				res = True
		elif operator is ">":
			res = varval > checkval
		elif operator is "<":
			res = varval < checkval
	else:
		if operator is "==":
			# round values so they are more comparable
			varval = round(varval, 3)
			checkval = round(checkval, 3)
			res = varval == checkval
		elif operator is ">":
			res = varval > checkval
		elif operator is "<":
			res = varval < checkval
	# res != neg allows neg to invert value of res
	return (res != neg, usesTime)
'''
Check conditional statement for this struct object
returns (isTrue, shouldDelete)
'''
def checkCond(block, data):
	condl = block['cond'].strip(' \t\n\r').split(" ")
	if len(condl) > 1 and len(condl)%2 == 0:
		print(block)
		print(condl)
		assert False # error improperly formatted condition
	i = 0
	ret = False
	op = None
	usesT = False
	for con in condl:
		if i%2 == 0:
			tempRet, hasT = checkCondSingle(con, data)
			usesT = usesT or hasT
			if op is None:
				ret = tempRet
			elif op is "&":
				if ret is False:
					break
			elif op is "|":
				if ret is True:
					break
			else:
				assert False # Bad logic operator
			ret = tempRet
		else:
			op = con
		i+=1
	return (ret, usesT)

'''
Handles the struct of instructions
Should a block be executed
'''
nextBlock = 0
def handleStruct(struct, data, replace):
	#global nextBlock
	if (struct is None):
		return
	# store the blocks that need to be deleted
	deletes = []
	for i, block in enumerate(struct):
		ret, usesT = checkCond(block, data)
		if ret is True:
			deleteB = executeBlock(block, data, replace)
			if usesT is True or deleteB is True:
				deleteB = True
				deletes.append(i)
	
	for i in deletes[::-1]:
		#print("DELETING i =", i)
		del struct[i]	

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
	#sys.stdout.write('%s|%s> <d:%.2f v:%.2f a:%.2f t:%d>\r' % (bar, spaces, dist, vel if abs(vel) >= 0.001 else 0, accel - accel_fric - accel_drag, epoch))
	sys.stdout.write('%s|%s> <d:%.2f v:%.2f a:%.2f t:%d>\r' % (bar, spaces, dist, vel if abs(vel) >= 0.001 else 0, data["net_accel"], epoch))
	sys.stdout.flush()
	#print(epoch, dist, vel, accel, "\r")
