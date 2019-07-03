from django.urls import path

from . import data_logic, actions_logic

urlpatterns = [
    path('data/battery', data_logic.battery, name="data-latest"),
    #path('data/warnings', data_logic.warnings, name="data-warnings"),
    #path('data/stats', data_logic.stats, name="data-stats"),
    #path('data/stats/essential', data_logic.essential_stats, name="data-essential-stats"),
    path('data/position', data_logic.position, name="data-position"),
    #path('commands/button', actions_logic.buttonPressed, name="command-button"),
    path('commands/dev', actions_logic.devCommand, name="command-dev"),
    #path('commands/servers', actions_logic.startupServers, name="startServers"),
    path('commands/test', data_logic.test, name="test")
]