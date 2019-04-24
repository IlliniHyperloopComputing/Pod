from django.urls import path
from . import tcpserver, udpserver

from . import views

urlpatterns = [
    path('data', views.getAllData, name='allData'),
    path('data/latest', views.getLatest, name='latest'),
    path('commands/stop', views.stopPressed, name='stopButton'),
    path('commands/ready', views.readyPressed, name='readyButton'),
    path('commands/dev', views.devCommand, name='devCommand')
]

tcpserver.start()
udpserver.start()