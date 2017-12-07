import sys
import time
from parser import parseFromFile 

delay = .05
numbars = 70

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
print("Simulation")

conditionals = [{"term":"t","val":"10", "thing":"a = -1"}];

def sign(a):
	if (a>0): return 1
	if (a==0): return 0
	return -1

def update():#(dist, vel, accel):
	global dist, vel, accel, deltaT, drag, delta_drag, fric, delta_fric
	# handle drag
	# accel due to drag
	
	# handle kinematics
	dist = dist + vel*deltaT + ((.5 * accel) * deltaT**2)
	vel = vel + accel*deltaT
	
	# handle drag/fric forces
	# follows this equation
	# Fdrag = ((1/2)*C*p*A) * Velocity**2
	# drag referes to the constant in this equation divided by weight of the pod
	accel_drag = drag * vel**2
	if (vel != 0 and abs(vel - sign(vel)*accel_drag*deltaT) > 0):
		vel = vel - sign(vel)*accel_drag*deltaT
		drag += delta_drag
		pass
	else:
		# drag will slow down pod to stop
		vel = 0
	# friction force
	# has equation Ffric = mu * Normal = fric*massOfPod
	if ( vel != 0 and abs(vel - sign(vel)*fric*deltaT) > 0):
		vel = vel - sign(vel)*fric*deltaT
		fric += delta_fric
		pass
	else:
		# fric will slow down pod to stop
		vel = 0
	
	if (dist > track_len):
		dist = track_len
		vel = 0
		accel = 0

	return (dist, vel)

def executeStatement(statement):
	global dist, vel, accel, deltaT, drag, delta_drag, fric, delta_fric
	varname = statement[0]
	value = float(statement[1])
	
	if (varname == "a" or varname == "%v"):
		accel = value
	elif (varname == "v" or varname == "%x"):
		vel = value
	elif (varname == "x"):
		dist = value
	elif (varname == "drag"):
		drag = value
	elif (varname == "%drag"):
		delta_drag = value
	elif (varname == "fric"):
		fric = value
	elif (varname == "%fric"):
		delta_fric = value
	

def executeBlock(block):
	global dist, vel, accel, deltaT, drag, delta_drag
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
		if ( float(condl[1]) - epoch < 0):
			executeBlock(block)
			nextBlock += 1

def printStatus():
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

	startTime = time.time()
	printStatus()
	oldTime = time.time()
	while epoch < 200:
		newTime = time.time()
		handleStruct(struct, epoch)
		deltaT = newTime-oldTime
		oldTime = time.time()
		update()#dist, vel, accel)
		epoch = epoch + deltaT
		printStatus()
	print()
