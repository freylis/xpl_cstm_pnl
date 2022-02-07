import xp

from . import base


class CommandVertTrimUp(base.Command):
    cmd = 'sim/flight_controls/pitch_trim_up'
    short_cmd = 'VTRIM_UP_ONE'


class CommandVertTrimDown(base.Command):
    cmd = 'sim/flight_controls/pitch_trim_down'
    short_cmd = 'VTRIM_DOWN_ONE'
