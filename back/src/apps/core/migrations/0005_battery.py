# Generated by Django 4.2.1 on 2023-05-21 13:19

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0004_alter_robot_ipaddr'),
    ]

    operations = [
        migrations.CreateModel(
            name='Battery',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('battery_num', models.IntegerField(blank=True, help_text='Остаток заряда аккумулятора', null=True, verbose_name='Остаток заряда аккумулятора')),
                ('robot', models.ForeignKey(help_text='Ссылка на робота', on_delete=django.db.models.deletion.CASCADE, related_name='battery', related_query_name='battery', to='core.robot', verbose_name='Ссылка на робота')),
            ],
            options={
                'verbose_name': 'Емкость батареи робота',
                'verbose_name_plural': 'Состояние батарей роботов',
            },
        ),
    ]