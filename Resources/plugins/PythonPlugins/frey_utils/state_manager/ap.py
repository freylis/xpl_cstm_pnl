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
    ref = 'laminar/B738/autopilot/speed_status1'


class StateVNAV(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/vnav_status1'


class StateLNAV(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/lnav_status'


class StateVORLOCK(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/vorloc_status'


class StateAPPROACH(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/app_status'


class StateCMD(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/cmd_a_status'


class StateCWS(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/cws_p_status'


class StateHeading(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/hdg_sel_status'


class StateAltitude(StateAPState):
    possible_indices = [-15]


class StateLevelChanged(base.StateSmallInt):
    ref = 'laminar/B738/autopilot/lvl_chg_status'


class StateTest(base.State):
    ref = 'sim/cockpit/autopilot/autopilot_state'

    def get_cmd_part(self):
        val = xp.getDatai(self.data_ref)
        return str(val)
