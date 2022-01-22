from . import base


class CommandGearUp(base.Command):
    short_cmd = 'GEARUP'
    cmd = 'sim/flight_controls/landing_gear_up'


class CommandGearDown(base.Command):
    short_cmd = 'GEARDOWN'
    cmd = 'sim/flight_controls/landing_gear_down'


class CommandGearToggle(base.Command):
    cmd = 'sim/flight_controls/landing_gear_toggle'
