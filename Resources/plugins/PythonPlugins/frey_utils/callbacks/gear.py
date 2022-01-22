import xp

from .. import commands
from .. import utils
from . import base


class CallbackGearUpDown(base.Callback):

    def callback(self, commandRef, phase, refCon):
        utils.echo(f'Callback for gear updown. p:{phase}')
        gear_ref = xp.findDataRef('sim/cockpit/switches/gear_handle_status')
        gear_ref_val = xp.getDatai(gear_ref)
        utils.echo(f'Current gear: {gear_ref_val}')

        if phase not in {1, '1'}:
            return

        if gear_ref_val in {1, '1'}:
            commands.CommandGearDown().send_command()
        elif gear_ref_val in {0, '0'}:
            commands.CommandGearUp().send_command()
        return 1
