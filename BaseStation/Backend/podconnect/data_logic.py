from django.shortcuts import render
from . import models
from . import tcpserver, udpserver
from django.http import HttpResponse, HttpRequest, JsonResponse
from django.core.serializers import serialize
import pickle
import struct

from threading import Lock
import json

TCPUp = False
UDPUp = False

def state(request):
    state_data = models.State.objects.latest("date_time")
    toReturn = {
        "value": state_data.state
    }
    return JsonResponse(toReturn)

def stats(request):
    state_data = models.State.objects.latest("date_time")
    can_data = models.CANData.objects.latest("date_time")
    toReturn = [{
        "stats":[
            {
                "name":"State",
                "value": state_data.state,
                "color": "limegreen"
            }, 
            {
                "name":"Status Word",
                "value": str(hex(can_data.status_word)),
                "color": "limegreen"
            },
            {
                "name": "Torque_Val",
                "value": str(can_data.torque_val),
                "color": "limegreen"
            },
            {
                "name": "Controller_Temp",
                "value": str(can_data.controller_temp),
                "color": "limegreen"
            },
            {
                "name":"Motor_Temp",
                "value":str(can_data.motor_temp),
                "color": "limegreen"
            },
            {
                "name":"Internal_Relay_State",
                "value":str(hex(can_data.internal_relay_state)),
                "color": "limegreen"
            },
            {
                "name":"Relay_State",
                "value": str(hex(can_data.relay_state)),
                "color": "limegreen"
            },
            {
                "name":"Rolling_Counter",
                "value": str(can_data.rolling_counter),
                "color": "limegreen"
            },
            {
                "name":"Fail_Safe_State",
                "value": str(hex(can_data.fail_safe_state)),
                "color": "limegreen"
            },
            {
                "name":"Peak_Current",
                "value": str(can_data.peak_current),
                "color": "limegreen"
            },
            {
                "name":"Pack_Voltage_Inst",
                "value": str(can_data.pack_voltage_inst),
                "color": "limegreen"
            }
            ]},
            {
            "stats": [{
                "name":"Highest_Temp",
                "value": str(can_data.highest_temp),
                "color": "red"
            },
            {
                "name":"Avg_Temp",
                "value": str(can_data.avg_temp),
                "color": "limegreen"
            },
            {
                "name":"Internal_Temp",
                "value": str(can_data.internal_temp),
                "color": "limegreen"
            },
            {
                "name":"Low_Cell_Voltage",
                "value": str(can_data.low_cell_voltage),
                "color": "limegreen"
            },
            {
                "name":"High_Cell_Voltage",
                "value": str(can_data.high_cell_voltage),
                "color": "limegreen"
            },
            {
                "name":"High_Cell_InternalR",
                "value": str(can_data.high_cell_internalR),
                "color": "limegreen"
            },
            {
                "name":"DTC_Status_One",
                "value": str(hex(can_data.dtc_status_one)),
                "color": "limegreen"
            },
            {
                "name":"DTC_Status_Two",
                "value": str(hex(can_data.dtc_status_two)),
                "color": "limegreen"
            }]
            }]
    return JsonResponse(toReturn, safe=False)

def battery(request):
    toReturn = {
        "value": 65
    }
    return JsonResponse(toReturn)

def position(request):
    can_data = models.CANData.objects.latest("date_time")
    toReturn = {
        "currentDistance":can_data.position_val,
        "totalDistance":1000
    }
    return JsonResponse(toReturn)

def stopPressed(request):
    if request.method == "POST":
        print("STOPPING")
        return HttpResponse()

def readyPressed(request):
    if request.method == "POST":
        print("Ready!")
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
