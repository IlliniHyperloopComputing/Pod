from . import models
import numpy as np

TEAM_ID = np.uint8(0)

def makeSpaceXPacket():
    to_send = b''
    to_send += TEAM_ID
    to_send += getStatus()
    to_send += getMotionData()
    to_send += getBatteryData()

def getStatus():
    return 0

# Get position in centimeters
def getMotionData():
    return 0

# Voltage Current Temperature
def getBatteryData():
    return 0

def getPotTemp():
    return 0