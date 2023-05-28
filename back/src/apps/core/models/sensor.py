from django.db import models

from src.apps.core.enums import SensorTypeEnum


class Sensor(models.Model):
    robot = models.ForeignKey(
        to='core.Robot',
        on_delete=models.CASCADE,
        related_name='sensor',
        related_query_name='sensor',
        verbose_name='Ссылка на робота',
        help_text='Ссылка на робота',
    )
    sensor_type = models.CharField(
        verbose_name="Тип датчика",
        help_text="Тип датчика",
        null=True,
        blank=True,
        max_length=100,
        choices=[{x.name, x.value} for x in SensorTypeEnum]
    )
    date_sensor = models.DateTimeField(
        verbose_name="Дата и время снятия показания",
        help_text="Дата и время снятия показания",
        null=True,
        blank=True,
        auto_created=True
    )
    value = models.IntegerField(
        verbose_name="Значение с датчика (см)",
        help_text="Значение с датчика (см)",
        null=True,
        blank=True
    )

    def __str__(self):
        return f"Робот {self.robot} - датчик {self.sensor_type}"

    class Meta:

        verbose_name = "Датчик робота"
        verbose_name_plural = "Датчики робота"