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
                print("Transitioning to Test Outside")
                tcpserver.addToCommandQueue([1])
            elif state_data.state == 1:
                print("Transitioning to Loading")
                tcpserver.addToCommandQueue([2])
            elif state_data.state == 2:
                print("Transitioning to Test Inside")
                tcpserver.addToCommandQueue([3])
            elif state_data.state == 3:
                print("Transitioning to Launch Ready")
                tcpserver.addToCommandQueue([4])
            elif state_data.state == 4:
                print("Transitioning to Ready")
                tcpserver.addToCommandQueue([5])
            else:
                print("State out of left button range")
        else:
            print("AGGHHHH STOP FAST")
            udpserver.addToCommandQueue([5])
    return HttpResponse(state_data.state)

def devCommand(request):
    if request.method == "POST":
        message = request.body.decode()
        mess = json.loads(message)
        command = int(mess["command"])
        if command == -1:
            return HttpResponse()
        print("Command " + str(command))
        if command == 0: #   TRANS_SAFE_MODE = 0,
            tcpserver.addToCommandQueue([0])
        elif command == 1: #   TRANS_FUNCTIONAL_TEST_OUTSIDE = 1, 
            tcpserver.addToCommandQueue([1])
        elif command == 2: #   TRANS_LOADING = 2,
            tcpserver.addToCommandQueue([2])
        elif command == 3: #   TRANS_FUNCTIONAL_TEST_INSIDE = 3,
            tcpserver.addToCommandQueue([3])
        elif command == 4: #   TRANS_LAUNCH_READY = 4,
            tcpserver.addToCommandQueue([4])
        elif command == 5: #   TRANS_FLIGHT_ACCEL = 5,
            tcpserver.addToCommandQueue([5])
        elif command == 6: #   ENABLE_MOTOR = 6,
            tcpserver.addToCommandQueue([6])
        elif command == 7: #   DISABLE_MOTOR = 7,
            tcpserver.addToCommandQueue([7])
        elif command == 8: #   SET_MOTOR_SPEED = 8,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([8, value])
        elif command == 9: #   ENABLE_BRAKE = 9,
            tcpserver.addToCommandQueue([9])
        elif command == 10: #   DISABLE_BRAKE = 10,
            tcpserver.addToCommandQueue([10])
        elif command == 11: #   TRANS_FLIGHT_COAST = 11,
            tcpserver.addToCommandQueue([11])
        elif command == 12: #   TRANS_FLIGHT_BRAKE = 12,
            tcpserver.addToCommandQueue([12])
        elif command == 13: #   TRANS_ABORT = 13,
            tcpserver.addToCommandQueue([13])
        elif command == 14: #   SET_ADC_ERROR = 14,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([14, value])
        elif command == 15: #   SET_CAN_ERROR = 15,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([15, value])
        elif command == 16: #   SET_I2C_ERROR = 16,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([16, value])
        elif command == 17: #   SET_PRU_ERROR = 17,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([17, value])
        elif command == 18: #   SET_NETWORK_ERROR = 18,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([18, value])
        elif command == 19: #   SET_OTHER_ERROR = 19,
            value = int(mess["value"])
            tcpserver.addToCommandQueue([19, value])
        elif command == 20: #   CLR_ADC_ERROR = 20,
            tcpserver.addToCommandQueue([20])
        elif command == 21: #   CLR_CAN_ERROR = 21,
            tcpserver.addToCommandQueue([21])
        elif command == 22: #   CLR_I2C_ERROR = 22,
            tcpserver.addToCommandQueue([22])
        elif command == 23: #   CLR_PRU_ERROR = 23,
            tcpserver.addToCommandQueue([23])
        elif command == 24: #   CLR_NETWORK_ERROR = 24,
            tcpserver.addToCommandQueue([24])
        elif command == 25: #   CLR_OTHER_ERROR = 25,
            tcpserver.addToCommandQueue([25])
        elif command == 26: #   SET_HV_RELAY_HV_POLE = 26,
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([26, value])
        elif command == 27: #   SET_HV_RELAY_LV_POLE = 27,
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([27, value])
        elif command == 28: #   SET_HV_RELAY_PRE_CHARGE = 28,
            print(mess)
            value = int(mess["value"])
            if value != 0 and value != 1:
                return HttpResponse("Value out of range")
            tcpserver.addToCommandQueue([28, value])
        elif command == 29: #   CALC_ACCEL_ZERO_G = 29,
            tcpserver.addToCommandQueue([29])

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
