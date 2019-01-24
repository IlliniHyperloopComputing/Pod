from django.shortcuts import render
from podconnect.models import DataPacket
from django.http import HttpResponse, HttpRequest
# Create your views here.
def index(request):
    return HttpResponse("Hello World!")