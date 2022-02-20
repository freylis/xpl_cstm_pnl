from . import base


class CommandAltitudeToggle(base.Command):
    short_cmd = 'AP_ALTITUDE_TOGGLE'
    cmd = 'laminar/B738/autopilot/alt_hld_press'


class CommandSetAltitude(base.CommandDataRefValue):
    short_cmd = 'AP_ALTITUDE_'
    cmd = 'laminar/B738/autopilot/mcp_alt_dial'


class CommandHeadingToggle(base.Command):
    short_cmd = 'AP_HEADING_TOGGLE'
    cmd = 'laminar/B738/autopilot/hdg_sel_press'


class CommandSetHeading(base.CommandDataRefValue):
    short_cmd = 'AP_HEADING_'
    cmd = 'laminar/B738/autopilot/mcp_hdg_dial'


class CommandSpeedToggle(base.Command):
    short_cmd = 'AP_SPEED_TOGGLE'
    cmd = 'laminar/B738/autopilot/speed_press'


class CommandSetSpeed(base.CommandDataRefValue):
    short_cmd = 'AP_SPEED_'
    cmd = 'sim/cockpit2/autopilot/airspeed_dial_kts_mach'


class CommandDisengage(base.Command):
    short_cmd = 'DISENGAGE_TOGGLE'
    cmd = 'laminar/B738/autopilot/left_at_dis_press'


class CommandCWSToggle(base.Command):
    short_cmd = 'CWS_TOGGLE'
    cmd = 'laminar/B738/autopilot/cws_a_press'


class CommandCMDToggle(base.Command):
    short_cmd = 'CMD_TOGGLE'
    cmd = 'laminar/B738/autopilot/cmd_a_press'


class CommandApproachToggle(base.Command):
    short_cmd = 'APPROACH_TOGGLE'
    cmd = 'laminar/B738/autopilot/app_press'


class CommandVORLOCToggle(base.Command):
    short_cmd = 'VOR_LOCK_TOGGLE'
    cmd = 'laminar/B738/autopilot/vorloc_press'


class CommandLNAVToggle(base.Command):
    short_cmd = 'LNAV_TOGGLE'
    cmd = 'laminar/B738/autopilot/lnav_press'


class CommandVNAVToggle(base.Command):
    short_cmd = 'LNAV_TOGGLE'
    cmd = 'laminar/B738/autopilot/vnav_press'


class CommandSetCourse(base.CommandDataRefValue):
    short_cmd = 'SET_COURSE_'
    cmd = 'laminar/B738/autopilot/course_pilot'

    def set_value(self, value):
        super().set_value(value)
        copilot_command = _COPilotCourse()
        copilot_command.set_value(value)


class _COPilotCourse(base.CommandDataRefValue):
    short_cmd = 'SET_COURSE'
    cmd = 'laminar/B738/autopilot/course_copilot'
