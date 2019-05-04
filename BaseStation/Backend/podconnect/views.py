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
    can_data = models.CANData.objects.latest("date_time")
    return HttpResponse("State: " 
        + str(state_data.state)
        + ", Status_Word: " + str(hex(can_data.status_word))
        + ", Torque_Val: " + str(can_data.torque_val)
        + ", Controller_Temp: " + str(can_data.controller_temp)
        + ", Motor_Temp: " + str(can_data.motor_temp)
        + ", DC_Link_Voltage: " + str(can_data.dc_link_voltage)
        + ", Internal_Relay_State: " + str(hex(can_data.internal_relay_state))
        + ", Relay_State: " + str(hex(can_data.relay_state))
        + ", Rolling_Counter: " + str(can_data.rolling_counter)
        + ", Fail_Safe_State: " + str(hex(can_data.fail_safe_state))
        + ", Peak_Current: " + str(can_data.peak_current)
        + ", Pack_Voltage_Inst " + str(can_data.pack_voltage_inst)
        + ", Highest_Temp: " + str(can_data.highest_temp)
        + ", Avg_Temp: " + str(can_data.avg_temp)
        + ", Internal_Temp: " + str(can_data.internal_temp)
        + ", Low_Cell_Voltage: " + str(can_data.low_cell_voltage)
        + ", High_Cell_Voltage: " + str(can_data.high_cell_voltage)
        + ", High_Cell_InternalR: " + str(can_data.high_cell_internalR)
        + ", DTC_Status_One: " + str(hex(can_data.dtc_status_one))
        + ", DTC_Status_Two: " + str(hex(can_data.dtc_status_two)))

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

    if not TCPUp:
        tcpserver.start()
        TCPUp = True
    if not UDPUp:
        udpserver.start()
        UDPUp = True
    return HttpResponse()