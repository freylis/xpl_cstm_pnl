import re

import xp
from .. import utils

from .. import commands


CMD_PATH = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_a.log'
command_regexp = re.compile(r'\[frey-cmd-a] (\w+)\s*', flags=re.I)


COMMANDS_MAPPING = {
    commands.CommandGearDown.short_cmd: commands.CommandGearDown,
    commands.CommandGearUp.short_cmd: commands.CommandGearUp,
}


def scheduled_callback(sinceLast, elapsedTime, counter, refCon):
    with open(CMD_PATH, 'r', encoding='utf-8') as f:
        command_lines = f.readlines()

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

    # utils.echo('Throttle changed')
    # throttle_ref = xp.findDataRef('sim/cockpit2/engine/actuators/throttle_ratio_all')
    # throttle_val = xp.getDataf(throttle_ref)
    #
    # thr_ref_2 = xp.findDataRef('sim/cockpit2/engine/actuators/throttle_jet_rev_ratio_all')
    # thr_ref_2_val = xp.getDataf(thr_ref_2)
    # utils.echo(f'Current throttle: {throttle_val} / {thr_ref_2_val}')
    return 1
