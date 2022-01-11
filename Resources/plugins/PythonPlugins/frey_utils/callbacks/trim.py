import xp

from . import base

from .. import utils


class CallbackVerticalTrimUpDown(base.Callback):

    @classmethod
    def callback(cls, commandRef, phase, refCon):
        trim_ref = xp.findDataRef('sim/flightmodel2/controls/stabilizer_deflection_degrees')
        trim_val = xp.getDataf(trim_ref)
        utils.echo(f'Vert trim val: {trim_val}')
        return 1