from django.db import models


class Robot(models.Model):

    ipaddr = models.CharField(
        verbose_name="IP-адресс устройства",
        help_text="IP-адрес устройства",
        max_length=16
    )
    token = models.CharField(
        verbose_name="Кодовое наименование устройства",
        help_text="Кодовое наименование устройства",
        max_length=16,
        null=True,
        blank=True
    )
    is_connected = models.BooleanField(
        verbose_name="Подключен ли робот",
        help_text="Подключен ли робот?",
        null=True,
        blank=True
    )
    first_connect = models.DateField(
        verbose_name="Первое подключение робота",
        help_text="Первое подключение робота",
        auto_created=True,
        null=True,
        blank=True
    )

    class Meta:

        verbose_name = "Робот"
        verbose_name_plural = "Роботы"