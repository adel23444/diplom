# Generated by Django 4.2.1 on 2023-05-28 17:38

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0009_sensor_value'),
    ]

    operations = [
        migrations.AddField(
            model_name='robot',
            name='manual_manage',
            field=models.BooleanField(default=False, help_text='Ручное управление роботом', verbose_name='Ручное управление роботом'),
        ),
    ]