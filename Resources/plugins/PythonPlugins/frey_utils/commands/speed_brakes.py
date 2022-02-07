import xp

from . import base


class CommandSpeedBrakeUp(base.Command):
    cmd = 'sim/flight_controls/speed_brakes_up_one'
    short_cmd = 'SPEEDBRAKES_UP_ONE'


class CommandSpeedBrakeDown(base.Command):
    cmd = 'sim/flight_controls/speed_brakes_down_one'
    short_cmd = 'SPEEDBRAKES_DOWN_ONE'
