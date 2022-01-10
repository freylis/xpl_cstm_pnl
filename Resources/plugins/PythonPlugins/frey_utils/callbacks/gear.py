import xp

from .. import utils
from . import base


class CallbackGearUpDown(base.Callback):

    def callback(self, commandRef, phase, refCon):
        utils.echo('Callback for gear updown')
        gear_ref = xp.findDataRef('sim/cockpit/switches/gear_handle_status')
        gear_ref_val = xp.getDatai(gear_ref)
        utils.echo(f'Current gear: {gear_ref_val}')
        return 1
