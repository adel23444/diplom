import json, base64, datetime
from django.http import JsonResponse, HttpResponse, HttpResponseRedirect
from django.shortcuts import render


from src.apps.mqtt.app_config import client as mqtt_client
from src.apps.core.enums import SensorTypeEnum
from src.apps.core.models import Robot, Sensor
from .forms import RobotForm


def get_sensor_data(sensors):
    k = 0
    sensor_data = []
    for i in range(len(sensors)-1, -1, -1):
        data = {
            "x": k,
            "y": sensors[i].value
        }
        sensor_data.append(data)
        k+=1

    return sensor_data

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
    if not Robot.objects.filter(id=robot_id).exists():
        return HttpResponseRedirect('/')
    robot = Robot.objects.get(id=robot_id)
    if robot.manual_manage == True:
        return HttpResponseRedirect(f'/robot_bluetooth/{robot_id}')
    sensor_left = Sensor.objects.filter(sensor_type=SensorTypeEnum.LEFT).order_by('date_sensor', '-id')[:10]
    sensor_left_data = get_sensor_data(sensor_left)
    sensor_prim = Sensor.objects.filter(sensor_type=SensorTypeEnum.PRIMARY).order_by('date_sensor', '-id')[:10]
    sensor_prim_data = get_sensor_data(sensor_prim)
    sensor_right = Sensor.objects.filter(sensor_type=SensorTypeEnum.RIGHT).order_by('date_sensor', '-id')[:10]
    sensor_right_data = get_sensor_data(sensor_right)

    context = {
        'robot': {
            'id': robot.id,
            'token': robot.token,
            'ipaddr': robot.ipaddr,
            'battery__battery_num': robot.battery.first().battery_num if robot.battery.all().exists() else "Не указано"
        },
        'sensor_left': sensor_left_data,
        'sensor_prim': sensor_prim_data,
        'sensor_right': sensor_right_data
    }
    print(context)
    return render(request, 'core/robot.html', context=context)


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


def sensor_data(request):
    type = request.GET.get('sensor_type')
    sensor = Sensor.objects.filter(sensor_type=type).order_by('-id')[0:10]

    sensordata = get_sensor_data(sensor)

    return HttpResponse(json.dumps(sensordata))

def robot_data(request):
    robot_id = request.GET.get('robot')
    robot = Robot.objects.get(id=robot_id)

    robot_data = {
        'battery': robot.battery.first().battery_num if robot.battery.all().exists() else "Не указано",
    }

    return HttpResponse(json.dumps(robot_data))


def deleterobot(request, robot_id):
    if Robot.objects.filter(id=robot_id).exists():
        Robot.objects.get(id=robot_id).delete()
    return HttpResponseRedirect('/')


def change_type_robot(request, robot_id):
    robot = Robot.objects.get(id=robot_id)
    robot.manual_manage = True
    robot.save()
    context = {
        'robot': robot
    }
    return render(request, 'core/bluetooth.html', context=context)