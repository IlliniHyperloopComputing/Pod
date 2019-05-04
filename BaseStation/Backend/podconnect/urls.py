from django.urls import path

from . import views

urlpatterns = [
    path('data/latest', views.getLatest, name='latest'),
    path('commands/stop', views.stopPressed, name='stopButton'),
    path('commands/ready', views.readyPressed, name='readyButton'),
    path('commands/dev', views.devCommand, name='devCommand'),
    path('commands/servers', views.startupServers, name='startServers')
]

