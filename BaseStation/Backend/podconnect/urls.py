from django.urls import path
from . import tcpserver

from . import views

urlpatterns = [
    path('', views.index, name='index'),
]

tcpserver.start()