import json, base64
from django.http import JsonResponse, HttpResponse
from django.shortcuts import render
from django.db.models import IntegerField, Value


from src.apps.mqtt.app_config import client as mqtt_client
from src.apps.core.models import Robot, Battery



def publish_message(request):
    request_data = json.loads(request.body)
    msg = base64.b64encode(bytes(request_data['msg'], 'utf-8'))
    rc, mid = mqtt_client.publish(request_data['topic'], msg)
    return JsonResponse({'code': rc})

def index(request):
    robots = Robot.objects.all()
    robots = robots.values('token', 'id', 'battery__battery_num', 'is_connected', 'ipaddr')
    context = {
        "robots": robots,
    }
    return render(request, 'core/index.html', context=context)