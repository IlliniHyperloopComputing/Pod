from django.urls import path
from . import tcpserver, udpserver

from . import views

urlpatterns = [
    path('data', views.getAllData, name='allData'),
    path('data/latest', views.getLatest, name='latest')
]

tcpserver.start()
udpserver.start()