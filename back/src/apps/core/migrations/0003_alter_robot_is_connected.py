# Generated by Django 4.2.1 on 2023-05-21 13:13

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0002_distance'),
    ]

    operations = [
        migrations.AlterField(
            model_name='robot',
            name='is_connected',
            field=models.BooleanField(default=False, help_text='Подключен ли робот?', null=True, verbose_name='Подключен ли робот'),
        ),
    ]
