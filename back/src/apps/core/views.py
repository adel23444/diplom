import json, base64
from django.http import JsonResponse, HttpResponse
from django.shortcuts import render
from src.apps.mqtt.app_config import client as mqtt_client


def publish_message(request):
    request_data = json.loads(request.body)
    msg = base64.b64encode(bytes(request_data['msg'], 'utf-8'))
    rc, mid = mqtt_client.publish(request_data['topic'], msg)
    return JsonResponse({'code': rc})

def index(request):
    return render(request, 'core/index.html')