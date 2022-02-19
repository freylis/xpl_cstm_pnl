import xp
from . import base


class StateCourse(base.StateSmallInt):
    ref = 'sim/cockpit/gps/course'


class StateAPSpeed(base.StateSmallInt):
    ref = 'sim/cockpit/autopilot/speed_status'

    def get_cmd_part(self):
        val = super().get_cmd_part()
        return '1' if val in ['1', '2'] else '0'


class StateVNAV(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/fms_vnav'


class StateLNAV(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/gpss_status'


class StateVORLOCK(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/dead_reckoning'


class StateAPPROACH(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/approach_status'


class StateCMD(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/autopilot_on'


class StateHeading(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/heading_status'


class StateAltitude(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/altitude_hold_status'


class StateCWS(base.State):
    ref = 'sim/cockpit2/autopilot/autopilot_on_or_cws'

    def get_cmd_part(self):
        state_cmd = StateCMD()
        cmd_enabled = xp.getDatai(state_cmd.data_ref)
        if cmd_enabled:
            return '0'
        cws_enabled = xp.getDatai(self.data_ref)
        return cws_enabled
