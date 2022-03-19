import re
import random

import xp
from .. import utils

from .. import commands


CMD_PATH = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_a.log'
command_regexp = re.compile(r'\[frey-cmd-a] (.+)\s*', flags=re.I)


COMMANDS_MAPPING = {
    commands.CommandGearDown.short_cmd: commands.CommandGearDown,
    commands.CommandGearUp.short_cmd: commands.CommandGearUp,
    commands.CommandFlapsUp.short_cmd: commands.CommandFlapsUp,
    commands.CommandFlapsDown.short_cmd: commands.CommandFlapsDown,
    commands.CommandSpeedBrakeUp.short_cmd: commands.CommandSpeedBrakeUp,
    commands.CommandSpeedBrakeDown.short_cmd: commands.CommandSpeedBrakeDown,
    commands.CommandVertTrimUp.short_cmd: commands.CommandVertTrimUp,
    commands.CommandVertTrimDown.short_cmd: commands.CommandVertTrimDown,
    # ap commands
    commands.CommandAltitudeToggle.short_cmd: commands.CommandAltitudeToggle,
    commands.CommandHeadingToggle.short_cmd: commands.CommandHeadingToggle,
    commands.CommandSpeedToggle.short_cmd: commands.CommandSpeedToggle,
    commands.CommandDisengage.short_cmd: commands.CommandDisengage,
    commands.CommandCMDToggle.short_cmd: commands.CommandCMDToggle,
    commands.CommandCWSToggle.short_cmd: commands.CommandCWSToggle,
    commands.CommandApproachToggle.short_cmd: commands.CommandApproachToggle,
    commands.CommandVORLOCToggle.short_cmd: commands.CommandVORLOCToggle,
    commands.CommandLNAVToggle.short_cmd: commands.CommandLNAVToggle,
    commands.CommandVNAVToggle.short_cmd: commands.CommandVNAVToggle,
    commands.CommandLevelChanged.short_cmd: commands.CommandLevelChanged,
}
VALUED_COMMANDS_MAPPING = (
    commands.CommandSetAltitude,
    commands.CommandSetHeading,
    commands.CommandSetSpeed,
    commands.CommandSetNav,
    commands.CommandAutoThrottle,
    commands.CommandFlightDirector,
    commands.CommandSetCourse,
)


def scheduled_callback(sinceLast, elapsedTime, counter, refCon):
    commands_found = False
    try:
        with open(CMD_PATH, 'r', encoding='utf-8') as f:
            command_lines = f.readlines()
    except FileNotFoundError:
        utils.echo(f'Not found file {CMD_PATH}')
        command_lines = []

    for index, command_string in enumerate(command_lines):
        commands_found = True
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
            continue

        # если команда на установку значения, найти ее просто перебором
        for v_cmd in VALUED_COMMANDS_MAPPING:
            if short_cmd.startswith(v_cmd.short_cmd):
                cmd = v_cmd()
                cmd.set_value(short_cmd)
                break
        else:
            utils.echo(f'Cant find command for {command_string!r}')
        continue

    if commands_found:
        with open(CMD_PATH, 'w', encoding='utf-8') as f:
            f.truncate()

    rnd = random.randint(0, 1)
    if rnd == 1:
        commands.CommandFullState().send_command()
    return 1
