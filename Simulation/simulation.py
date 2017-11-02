import sys
import time
delay = .05
numbars = 70

dist = 0.0
track_len = 1400.0
vel = 0.0
accel = 1
deltaT = 0.1
epoch = 0.0
print("Simulation")
conditionals = [{"term":"t","val":"10", "thing":"a = -1"}];
def update():#(dist, vel, accel):
	global dist, vel, accel, deltaT
	dist = dist + vel*deltaT + ((.5 * accel) * deltaT**2)
	vel = vel + accel*deltaT
	if(dist > track_len):
		dist = track_len
		vel = 0
		accel = 0
	
	# check conditionals from parser.py
	for cond in conditionals:
		if cond["term"] == "t":
			if epoch == float(cond["val"]):
				pass
				# do thing
	return (dist, vel)

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

printStatus()
oldTime = time.time()
while epoch < 20:
	newTime = time.time()
	deltaT = newTime-oldTime
	oldTime = time.time();
	if(dist > 487):
		brake()
	update()#dist, vel, accel)
	epoch = epoch + deltaT
	printStatus()
print()
