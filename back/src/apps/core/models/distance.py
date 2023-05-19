from django.db import models


class Distance(models.Model):

    robot = models.ForeignKey(
        to='core.Robot',
        on_delete=models.CASCADE,
        related_name='distance',
        related_query_name='distance',
        verbose_name='Ссылка на робота',
        help_text='Ссылка на робота',
    )
    path = models.IntegerField(
        verbose_name="Расстояние (в см)",
        help_text="Расстояние (в см)",
        null=True,
        blank=True
    )
    date_distance = models.DateTimeField(
        verbose_name="Дата и время прохождения",
        help_text="Дата и время прохождения",
        null=True,
        blank=True,
        auto_created=True
    )
    speed = models.FloatField(
        verbose_name="Скорость робота",
        help_text="Скорость робота",
        null=True,
        blank=True
    )
    direction = models.CharField(
        help_text="Направление движения",
        verbose_name="Направление движения",
        null=True,
        blank=True,
        max_length=16
    )

    class Meta:
        verbose_name = 'Расстояние робота',
        verbose_name_plural = "Список движений роботов"