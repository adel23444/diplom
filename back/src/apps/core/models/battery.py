from django.db import models

class Battery(models.Model):

    robot = models.ForeignKey(
        to='core.Robot',
        on_delete=models.CASCADE,
        related_name='battery',
        related_query_name='battery',
        verbose_name='Ссылка на робота',
        help_text='Ссылка на робота',
    )
    battery_num = models.IntegerField(
        verbose_name="Остаток заряда аккумулятора",
        help_text="Остаток заряда аккумулятора",
        null=True,
        blank=True
    )

    class Meta:
        verbose_name = "Емкость батареи робота"
        verbose_name_plural = "Состояние батарей роботов"