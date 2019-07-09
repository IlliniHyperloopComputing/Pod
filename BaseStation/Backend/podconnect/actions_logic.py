from . import tcpserver, udpserver, models
import json
from django.http import HttpResponse, HttpRequest, JsonResponse

TCPUp = False
UDPUp = False

def buttonPressed(request):
    state_data = models.State.objects.latest("date_time")
    if request.method == "POST":
        message = request.body.decode()
        mess = json.loads(message)
        if mess["button"] == "ready":
            if state_data.state == 0:
                print("Transitioning to Test")
                tcpserver.addToCommandQueue([1, 0])
            elif state_data.state == 1:
                print("Transitioning to Loading")
                tcpserver.addToCommandQueue([2, 0])
            elif state_data.state == 2:
                print("Transitioning to Launch Ready")
                tcpserver.addToCommandQueue([3, 0])
            elif state_data.state == 3:
                print("Transitioning to Launch")
                tcpserver.addToCommandQueue([4, 0])
            else:
                print("State out of left button range")
        else:
            print("AGGHHHH STOP FAST")
            tcpserver.addToCommandQueue([5, 0])
    return HttpResponse(state_data.state)

#  TRANS_SAFE_MODE = 0,
#  TRANS_FUNCTIONAL_TEST = 1,
#  TRANS_LOADING = 2,
#  TRANS_LAUNCH_READY = 3,
#  LAUNCH = 4,
#  EMERGENCY_BRAKE = 5,
#  ENABLE_MOTOR = 6,
#  DISABLE_MOTOR = 7,
#  SET_MOTOR_SPEED = 8,
#  ENABLE_BRAKE = 9,
#  DISABLE_BRAKE = 10,
#  TRANS_FLIGHT_COAST = 11,
#  TRANS_FLIGHT_BRAKE = 12,
#  TRANS_ERROR_STATE = 13,
#  SET_ADC_ERROR = 14,  // SET_XXX_ERROR defines must be one after another, in one block
#  SET_CAN_ERROR = 15,
#  SET_I2C_ERROR = 16,
#  SET_PRU_ERROR = 17,
#  SET_NETWORK_ERROR = 18,
#  SET_OTHER_ERROR = 19,
#  CLR_ADC_ERROR = 20,  // CLR_XXX_ERROR defines must be one after another, in one block
#  CLR_CAN_ERROR = 21,
#  CLR_I2C_ERROR = 22,
#  CLR_PRU_ERROR = 23,
#  CLR_NETWORK_ERROR = 24,
#  CLR_OTHER_ERROR = 25,
#  SET_HV_RELAY_HV_POLE = 26,
#  SET_HV_RELAY_LV_POLE = 27,
#  SET_HV_RELAY_PRE_CHARGE = 28,
def devCommand(request):
    if request.method == "POST":
        message = request.body.decode()
        mess = json.loads(message)
        command = int(mess["command"])
        if command == -1:
            return HttpResponse()
        print("Command " + str(command))
        if command == 0:
            print(mess)
            tcpserver.addToCommandQueue([0, 0])
        if command == 1:
            print(mess)
            tcpserver.addToCommandQueue([1, 0])
        if command == 6:
            print(mess)
            tcpserver.addToCommandQueue([6, 0])
        if command == 7:
            print(mess)
            tcpserver.addToCommandQueue([7, 0])
        if command == 8:
            print(mess)
            value = int(mess["value"])
            tcpserver.addToCommandQueue([8, value])
        if command == 26:
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([26, value])
        if command == 27:
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([27, value])
        if command == 28:
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([28, value])

        return HttpResponse()
    return HttpResponse()

def startupServers(request):
    global TCPUp, UDPUp
    if request.method == "POST":
        if not TCPUp:
            tcpserver.start()
            TCPUp = True
        if not UDPUp:
            udpserver.start()
            UDPUp = True
        return HttpResponse()
