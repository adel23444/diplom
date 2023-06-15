import enum


class SensorTypeEnum(object):

    LEFT = 0
    PRIMARY = 1
    RIGHT = 2

    CHOICES = (
        (LEFT, 'Левый борт'),
        (PRIMARY, 'Передняя часть'),
        (RIGHT, 'Правый борт')
    )