from django.contrib import admin

from .models import (Robot, Distance,
                     Battery, Sensor)

admin.site.register(Robot)
admin.site.register(Distance)
admin.site.register(Battery)
admin.site.register(Sensor)