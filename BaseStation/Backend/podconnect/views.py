from django.shortcuts import render
from podconnect.models import DataPacket
from django.http import HttpResponse, HttpRequest
from django.core.serializers import serialize

from threading import Lock

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