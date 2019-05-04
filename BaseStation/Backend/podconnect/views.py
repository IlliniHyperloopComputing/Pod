from django.shortcuts import render
from . import models
from . import tcpserver, udpserver
from django.http import HttpResponse, HttpRequest
from django.core.serializers import serialize
import pickle
import struct

from threading import Lock
import json

TCPUp = False
UDPUp = False

# Might need mutex locks if db doesnt handle concurrency
def getLatest(request):
    # can_data = models.CANData.objects.latest("date_time")
    # error_data = models.Errors.objects.latest("date_time")
    state_data = models.State.objects.latest("date_time")
    return HttpResponse("State: " + state_data.state)

def stopPressed(request):
    if request.method == "POST":
        print("STOPPING")
        return HttpResponse()

def readyPressed(request):
    if request.method == "POST":
        print("Ready!")
        return HttpResponse()

def devCommand(request):
    if request.method == "POST":
        message = request.body.decode()
        mess = json.loads(message)
        command = int(mess["command"])
        print("Command " + str(command))
        if command == 1:
            print(mess)
            value = int(mess["value"])
            if value > 8 or value < 0:
                return HttpResponse("Failed")
            tcpserver.addToCommandQueue([1, value])
        if command == 2:
            print(mess)
            value = int(mess["value"])
            if value > 8 or value < 0:
                return HttpResponse("Failed")
            tcpserver.addToCommandQueue([1, value])
        return HttpResponse()

def startupServers(request):
    global TCPUp, UDPUp

    if not TCPUp:
        tcpserver.start()
        TCPUp = True
    if not UDPUp:
        udpserver.start()
        UDPUp = True
    return HttpResponse()