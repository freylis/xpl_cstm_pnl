import xp

from . import base


class CommandSpeedBrakeUp(base.Command):
    cmd = 'sim/flight_controls/speed_brakes_up_one'


class CommandSpeedBrakeDown(base.Command):
    cmd = 'sim/flight_controls/speed_brakes_down_one'
