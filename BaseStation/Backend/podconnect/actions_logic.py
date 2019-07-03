from . import tcpserver
import json
from django.http import HttpResponse, HttpRequest, JsonResponse

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
