from django.db import models
from django.core.validators import MinValueValidator, MaxValueValidator

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
        blank=True,
        validators=[MinValueValidator(0), MaxValueValidator(100)],
    )

    def __str__(self):
        return f"Робот {self.robot.token} - остаток заряда {self.battery_num}%"

    class Meta:
        verbose_name = "Емкость батареи робота"
        verbose_name_plural = "Состояние батарей роботов"