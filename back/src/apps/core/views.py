import json, base64, datetime
from django.http import JsonResponse, HttpResponse, HttpResponseRedirect
from django.shortcuts import render


from src.apps.mqtt.app_config import client as mqtt_client
from src.apps.core.models import Robot
from .forms import RobotForm



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


def robot(request, robot_id):
    return HttpResponse(f"Робот с ID {robot_id}")


def addrobot(request):
    if request.method == 'POST':
        form = RobotForm(request.POST)
        if form.is_valid():
            Robot.objects.create(
                ipaddr=form.cleaned_data['ipaddr'],
                token=form.cleaned_data['token'],
                is_connected=False,
                first_connect=datetime.date.today()
            )
            return HttpResponseRedirect('/')
    return render(request, 'core/create.html')
