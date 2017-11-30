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
print("Simulation")

conditionals = [{"term":"t","val":"10", "thing":"a = -1"}];

def update():#(dist, vel, accel):
	global dist, vel, accel, deltaT, drag, delta_drag
	# handle drag
	# accel due to drag
	'''
	draga = drag * vel**2
	if (abs(accel) - draga > 0):
		if (
	else:
		accel = 0
	'''
	
	# handle kinematics
	dist = dist + vel*deltaT + ((.5 * accel) * deltaT**2)
	vel = vel + accel*deltaT
	if(dist > track_len):
		dist = track_len
		vel = 0
		accel = 0
	'''
	# check conditionals from parser.py
	for cond in conditionals:
		if cond["term"] == "t":
			if epoch == float(cond["val"]):
				pass
				# do thing
	'''
	return (dist, vel)

def executeBlock(block):
	global dist, vel, accel, deltaT, drag, delta_drag
	stat = block['stat']
	for s in stat:
		s = s.split('=')
		varname = s[0]	
		value = float(s[1])
		
		if (varname == "a" or varname == "%v"):
			print("TEST STUFF\n")
			accel = value
		elif (varname == "v" or varname == "%x"):
			vel = value
		elif (varname == "x"):
			dist = value
		elif (varname == "drag"):
			drag = value
		elif (varname == "%drag"):
			drag = value
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
		#print(newTime - float(condl[1]) - startTime)
		if ( startTime + float(condl[1]) - newTime < 0):
			executeBlock(block)
			nextBlock += 1

def printStatus():
	drawBars = int(((dist)/track_len)*numbars)
	bar = "="*drawBars
	spaces = ""
	if numbars - drawBars > 0:
		spaces = "_"*(numbars-drawBars)
	sys.stdout.write('%s|%s> <d:%.2f v:%.2f a:%.2f t:%d>\r' % (bar, spaces, dist, vel, accel, epoch))
	sys.stdout.flush()
	#print(epoch, dist, vel, accel, "\r")

def brake():
	global accel
	if (vel > 0):
		accel = -.1;
	else:
		accel = 0;

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
	while epoch < 20:
		newTime = time.time()
		handleStruct(struct, newTime)
		deltaT = newTime-oldTime
		oldTime = time.time()
		if(dist > 487):
			brake()
		update()#dist, vel, accel)
		epoch = epoch + deltaT
		printStatus()
	print()
