# Generated by Django 4.2.1 on 2023-05-22 10:42

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0006_alter_battery_battery_num'),
    ]

    operations = [
        migrations.CreateModel(
            name='Sensor',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('sensor_type', models.CharField(blank=True, choices=[{'Левый борт', 'LEFT'}, {'PRIMARY', 'Передняя часть'}, {'RIGHT', 'Правый борт'}], help_text='Тип датчика', max_length=100, null=True, verbose_name='Тип датчика')),
                ('robot', models.ForeignKey(help_text='Ссылка на робота', on_delete=django.db.models.deletion.CASCADE, related_name='sensor', related_query_name='sensor', to='core.robot', verbose_name='Ссылка на робота')),
            ],
            options={
                'verbose_name': 'Датчик робота',
                'verbose_name_plural': 'Датчики робота',
            },
        ),
    ]
