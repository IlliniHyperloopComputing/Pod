from django.db import models

# Create your models here.
class Data(models.Model):
    recv_time = models.IntegerField(default=0.0)
    state = models.CharField(max_length=200)
    position = models.FloatField(default=0.0)
    position.null = True
    velocity = models.FloatField(default=0.0)
    velocity.null = True
    acceleration = models.FloatField(default=0.0)
    acceleration.null = True
    cpu_temp = models.FloatField(default=0.0)
    cpu_temp.null = True

    def __str__(self):
        return str(self.recv_time)