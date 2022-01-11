from . import base


class CommandThrottleUp(base.Command):
    cmd = 'sim/engines/throttle_up'


class CommandThrottleDown(base.Command):
    cmd = 'sim/engines/throttle_down'


class CommandEngineFirstUp(base.Command):
    cmd = 'sim/flight_controls/water_rudder_up'


class CommandEngineFirstDown(base.Command):
    cmd = 'sim/flight_controls/rotor_rpm_trim_up'


class CommandEngineV1(base.Command):
    cmd = 'sim/flight_controls/pitch_trimA_up'


class CommandEngineV2(base.Command):
    cmd = 'laminar/B738/engine/thr_lvr1'


class CommandEngineV3(base.Command):
    cmd = 'laminar/B738/engine/thr_lvr2'


class CommandEngineV4(base.Command):
    cmd = 'sim/flight_controls/rotor_rpm_trim_up'


# class CommandEngineFirstDown(base.Command):
#     cmd = '	sim/engines/throttle_down_1'
