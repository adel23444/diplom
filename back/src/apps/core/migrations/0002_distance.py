# Generated by Django 4.2.1 on 2023-05-19 23:13

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Distance',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('date_distance', models.DateTimeField(auto_created=True, blank=True, help_text='Дата и время прохождения', null=True, verbose_name='Дата и время прохождения')),
                ('path', models.IntegerField(blank=True, help_text='Расстояние (в см)', null=True, verbose_name='Расстояние (в см)')),
                ('speed', models.FloatField(blank=True, help_text='Скорость робота', null=True, verbose_name='Скорость робота')),
                ('direction', models.CharField(blank=True, help_text='Направление движения', max_length=16, null=True, verbose_name='Направление движения')),
                ('robot', models.ForeignKey(help_text='Ссылка на робота', on_delete=django.db.models.deletion.CASCADE, related_name='distance', related_query_name='distance', to='core.robot', verbose_name='Ссылка на робота')),
            ],
            options={
                'verbose_name': ('Расстояние робота',),
                'verbose_name_plural': 'Список движений роботов',
            },
        ),
    ]
