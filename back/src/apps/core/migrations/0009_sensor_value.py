# Generated by Django 4.2.1 on 2023-05-27 20:08

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0008_sensor_date_sensor'),
    ]

    operations = [
        migrations.AddField(
            model_name='sensor',
            name='value',
            field=models.IntegerField(blank=True, help_text='Значение с датчика (см)', null=True, verbose_name='Значение с датчика (см)'),
        ),
    ]