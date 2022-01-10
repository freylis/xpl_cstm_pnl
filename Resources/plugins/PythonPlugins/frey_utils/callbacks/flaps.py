import xp

from .. import utils
from . import base


class CallbackFlapsUpDown(base.Callback):

    @classmethod
    def callback(cls, commandRef, phase, refCon):
        utils.echo('Callback for flaps updown')
        flap_ref = xp.findDataRef('sim/flightmodel/controls/flaprqst')
        flap_val = xp.getDataf(flap_ref)
        utils.echo(f'Current flaps: {flap_val}')
        return 1
