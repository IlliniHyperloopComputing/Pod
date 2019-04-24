from django.shortcuts import render
from podconnect.models import DataPacket
from django.http import HttpResponse, HttpRequest
from django.core.serializers import serialize

from threading import Lock
import json

def getAllData(request):
    mutex = Lock()
    mutex.acquire(1)
    e = DataPacket.objects.all()
    mutex.release()
    xmlE = serialize("xml",e)
    return HttpResponse(xmlE)

def getLatest(request):
    mutex = Lock()
    mutex.acquire(1)
    e = DataPacket.objects.latest("date_time")
    mutex.release()
    xmlE = serialize("xml",[e])
    return HttpResponse(xmlE)

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
        print("Command " + str(mess))
        return HttpResponse()