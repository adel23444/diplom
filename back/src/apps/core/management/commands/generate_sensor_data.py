import random, time, datetime
from django.core.management import BaseCommand


from src.apps.core.models import Sensor, Robot
from src.apps.core.enums import SensorTypeEnum


class Command(BaseCommand):

    def handle(self, *args, **options):
        robot = Robot.objects.first()
        for i in range(0, 100):
            Sensor.objects.create(
                robot=robot,
                sensor_type=SensorTypeEnum.LEFT.name,
                value=random.randint(15, 36),
                date_sensor=datetime.datetime.now()
            )
            time.sleep(1)
            Sensor.objects.create(
                robot=robot,
                sensor_type=SensorTypeEnum.PRIMARY.name,
                value=random.randint(15, 20),
                date_sensor=datetime.datetime.now()
            )
            time.sleep(1)
            Sensor.objects.create(
                robot=robot,
                sensor_type=SensorTypeEnum.RIGHT.name,
                value=random.randint(15, 36),
                date_sensor=datetime.datetime.now()
            )
            time.sleep(1)

        print('DONE!')
