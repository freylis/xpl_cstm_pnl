import xp

from .. import utils
from . import base


class CallbackSpeedBrakesUpDown(base.Callback):

    @classmethod
    def callback(cls, commandRef, phase, refCon):
        sb_ref = xp.findDataRef('sim/cockpit2/controls/speedbrake_ratio')
        sb_val = xp.getDataf(sb_ref)
        utils.echo(f'Current speedbrakes: {sb_val}')
        return 1
