from django.db import models
from datetime import datetime

class Command(models.Model):
    name = models.CharField(max_length=30)
    description = models.CharField(max_length=255)
    command = models.CharField(max_length=20)
    def __unicode__(self):
        return self.name

class Execute(models.Model):
    command = models.ForeignKey(Command)
    date = models.DateTimeField(default=datetime.now())
    completed = models.SmallIntegerField(default=0)
    def __unicode__(self):
        return self.command.name

class RefSensorType(models.Model):
    sensorType = models.CharField(max_length=30)
    def __unicode__(self):
        return self.sensorType

class RefState(models.Model):
    sensorType = models.ForeignKey(RefSensorType)
    state = models.CharField(max_length=20)
    displayName = models.CharField(max_length=30)
    css = models.CharField(max_length=30)
    def __unicode__(self):
        return self.state

class Sensor(models.Model):
    sensorType = models.ForeignKey(RefSensorType)
    name = models.CharField(max_length=30)
    displayName = models.CharField(max_length=30)
    pin = models.SmallIntegerField(default=0)
    def __unicode__(self):
        return self.name

class Event(models.Model):
    date = models.DateTimeField(default=datetime.now())
    sensor = models.ForeignKey(Sensor)
    state = models.ForeignKey(RefState)
    def __unicode__(self):
        return self.sensor.name + " " + self.state.state

class Parameter(models.Model):
    key = models.CharField(max_length=50)
    group = models.CharField(max_length=50)
    value = models.CharField(max_length=250)
    name = models.CharField(max_length=30)
    description = models.CharField(max_length=255,blank=True, null=True)
    showInUI = models.SmallIntegerField(default=0)
    order = models.SmallIntegerField(default=0)
    def __unicode__(self):
        return self.key

class ZMIntrusion(models.Model):
    date = models.DateTimeField(default=datetime.now())

class ZMIntrusionPicture(models.Model):
    zmIntrusion = models.ForeignKey(ZMIntrusion)
    path = models.CharField(max_length=250)
    def __unicode__(self):
        return self.path

class Security(models.Model):
	camera = models.IntegerField() 
	filename = models.CharField(max_length=80)
	frame = models.IntegerField() 
	file_type = models.IntegerField() 
	time_stamp = models.DateTimeField()
	text_event = models.DateTimeField()
	def __unicode__(self):
		return self.filename
		
class MotionEventPicture(models.Model):
	event = models.ForeignKey(Event)
	security = models.ForeignKey(Security)
