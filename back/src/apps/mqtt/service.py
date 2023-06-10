from .commands_const import CommandConstEnum

def parse(string: str) -> dict:
    commands = [x.value for x in CommandConstEnum]
    robot_name = ''
    robot_command = ''
    value = 0

    for command in commands:
        if string.find(command):
            ind = string.find(command)
            robot_name = string[0:ind]
            if command == 'OK' and command in string:
                robot_complete = True
            else:
                value = string[ind + len(command):len(string) - 1]
            robot_command = command
            break

    robot_dict = {
        'name': robot_name,
        'command': robot_command,
        'value': value,
        'command_complete': robot_complete if robot_complete else None
    }

    return robot_dict
