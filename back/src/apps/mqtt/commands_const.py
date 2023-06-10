import enum

class CommandConstEnum(enum.Enum):

    BATTERY = 'BATT'
    WIFI = 'WIFI'
    BLUETOOTH = 'BLTH'
    LEFT_SENSOR = 'SENL'
    RIGHT_SENSOR = 'SENR'
    PRIMARY_SENSOR = 'SENP'
    LEFT_MOVE = 'MOVL'
    RIGHT_MOVE = 'MOVR'
    PRIMARY_MOVE = 'MOVP'
    BACKWARD_MOVE = 'MOVB'
    LEFT_MOVE_SERVER = 'MOVLS'
    RIGHT_MOVE_SERVER = 'MOVRS'
    PRIMARY_MOVE_SERVER = 'MOVPS'
    BACKWARD_MOVE_SERVER = 'MOVBS'
    COMMAND_DONE = 'OK'