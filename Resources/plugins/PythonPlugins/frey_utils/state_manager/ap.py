import xp
from . import base


class StateAPState(base.State):
    ref = 'sim/cockpit/autopilot/autopilot_state'
    possible_indices = []

    def get_cmd_part(self):
        dec_ap_state = xp.getDatai(self.data_ref)
        bin_data = bin(dec_ap_state)
        for pi in self.possible_indices:
            if len(bin_data) - 2 >= abs(pi) and bin_data[pi] == '1':
                return '1'
        return '0'


class StateCourse(base.StateSmallInt):
    ref = 'sim/cockpit/gps/course'


class StateAPSpeed(base.StateSmallInt):
    ref = 'sim/cockpit/autopilot/airspeed'


class StateVNAV(StateAPState):
    possible_indices = [-13, -14]


class StateLNAV(StateAPState):
    possible_indices = [-17, -18]


class StateVORLOCK(StateAPState):
    possible_indices = [-9, -10]


class StateAPPROACH(StateAPState):
    possible_indices = [-11, -12]


class StateCMD(base.StateSmallInt):
    ref = 'sim/cockpit2/autopilot/autopilot_on'


class StateHeading(StateAPState):
    possible_indices = [-2]


class StateAltitude(StateAPState):
    possible_indices = [-15]


class StateCWS(base.State):
    ref = 'sim/cockpit2/autopilot/autopilot_on_or_cws'

    def get_cmd_part(self):
        state_cmd = StateCMD()
        cmd_enabled = str(xp.getDatai(state_cmd.data_ref))
        if cmd_enabled:
            return '0'
        cws_enabled = str(xp.getDatai(self.data_ref))
        return cws_enabled


class StateTest(base.State):
    ref = 'sim/cockpit/autopilot/autopilot_state'

    def get_cmd_part(self):
        val = xp.getDatai(self.data_ref)
        return str(val)
