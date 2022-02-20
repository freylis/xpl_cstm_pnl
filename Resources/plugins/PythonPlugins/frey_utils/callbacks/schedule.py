import re

import xp
from .. import utils

from .. import commands


CMD_PATH = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_a.log'
command_regexp = re.compile(r'\[frey-cmd-a] (\w+)\s*', flags=re.I)


COMMANDS_MAPPING = {
    commands.CommandGearDown.short_cmd: commands.CommandGearDown,
    commands.CommandGearUp.short_cmd: commands.CommandGearUp,
    commands.CommandFlapsUp.short_cmd: commands.CommandFlapsUp,
    commands.CommandFlapsDown.short_cmd: commands.CommandFlapsDown,
    commands.CommandSpeedBrakeUp.short_cmd: commands.CommandSpeedBrakeUp,
    commands.CommandSpeedBrakeDown.short_cmd: commands.CommandSpeedBrakeDown,
    commands.CommandVertTrimUp.short_cmd: commands.CommandVertTrimUp,
    commands.CommandVertTrimDown.short_cmd: commands.CommandVertTrimDown,
}


def scheduled_callback(sinceLast, elapsedTime, counter, refCon):
    try:
        with open(CMD_PATH, 'r', encoding='utf-8') as f:
            command_lines = f.readlines()
    except FileNotFoundError:
        utils.echo(f'Not found file {CMD_PATH}')
        command_lines = []

    for index, command_string in enumerate(command_lines):
        command_match = command_regexp.search(command_string)
        if not command_match:
            utils.echo(f'Unknown command {command_string}')
            continue

        short_cmd = command_match.groups()[0]
        # try to find in simple commands
        if short_cmd in COMMANDS_MAPPING:
            cmd = COMMANDS_MAPPING[short_cmd]()
            utils.echo(f'Execute command {index}: {cmd}')
            cmd.execute()

    with open(CMD_PATH, 'w', encoding='utf-8') as f:
        f.truncate()

    commands.CommandFullState().send_command()
    return 1
