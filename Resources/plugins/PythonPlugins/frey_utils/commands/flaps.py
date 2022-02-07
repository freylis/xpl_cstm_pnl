from . import base


class CommandFlapsUp(base.Command):
    cmd = 'sim/flight_controls/flaps_up'
    short_cmd = 'FLAPS_UP_ONE'


class CommandFlapsDown(base.Command):
    cmd = 'sim/flight_controls/flaps_down'
    short_cmd = 'FLAPS_DOWN_ONE'
