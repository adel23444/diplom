from django.contrib import admin

from .models import Robot, Distance, Battery

admin.site.register(Robot)
admin.site.register(Distance)
admin.site.register(Battery)