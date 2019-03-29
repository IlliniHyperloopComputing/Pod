from django.db import models

# Create your models here.

class DataPacket(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    velocity = models.IntegerField(null=True)
    acceleration = models.IntegerField(null=True)
    position = models.IntegerField(null=True)