import re
import xp

from .. import utils
from . import base


class CommandAltitudeToggle(base.Command):
    short_cmd = 'AP_ALTITUDE_TOGGLE'
    cmd = 'laminar/B738/autopilot/alt_hld_press'


class CommandSetAltitude(base.CommandDataRefIntegerValue):
    short_cmd = 'AP_ALTITUDE_'
    cmd = 'laminar/B738/autopilot/mcp_alt_dial'


class CommandHeadingToggle(base.Command):
    short_cmd = 'AP_HEADING_TOGGLE'
    cmd = 'laminar/B738/autopilot/hdg_sel_press'


class CommandSetHeading(base.CommandDataRefIntegerValue):
    short_cmd = 'AP_HEADING_'
    cmd = 'laminar/B738/autopilot/mcp_hdg_dial'


class CommandSpeedToggle(base.Command):
    short_cmd = 'AP_SPEED_TOGGLE'
    cmd = 'laminar/B738/autopilot/speed_press'


class CommandSetSpeed(base.CommandDataRefFloatValue):
    short_cmd = 'AP_SPEED_'
    cmd = "laminar/B738/autopilot/mcp_speed_dial_kts_mach"


class CommandDisengage(base.Command):
    short_cmd = 'DISENGAGE_TOGGLE'
    # cmd = 'laminar/B738/autopilot/left_at_dis_press'
    cmd = 'laminar/B738/autopilot/disconnect_toggle'


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
    short_cmd = 'VNAV_TOGGLE'
    cmd = 'laminar/B738/autopilot/vnav_press'


class COPilotCourse(base.CommandDataRefIntegerValue):
    short_cmd = 'SET_COURSE'
    cmd = 'laminar/B738/autopilot/course_copilot'


class CommandSetCourse(base.CommandDataRefIntegerValue):
    short_cmd = 'SET_COURSE_'
    cmd = 'laminar/B738/autopilot/course_pilot'

    def set_value(self, value):
        super().set_value(value)
        copilot_command = COPilotCourse()
        copilot_command.set_value(value)


class CommandNavActiveMhz(base.CommandDataRefIntegerValue):
    cmd = 'sim/cockpit2/radios/actuators/nav1_frequency_Mhz'


class CommandNavActiveKhz(base.CommandDataRefIntegerValue):
    cmd = 'sim/cockpit2/radios/actuators/nav1_frequency_khz'


class CommandNavStandByMhz(base.CommandDataRefIntegerValue):
    cmd = 'sim/cockpit2/radios/actuators/nav1_standby_frequency_Mhz'


class CommandNavStandByKhz(base.CommandDataRefIntegerValue):
    cmd = 'sim/cockpit2/radios/actuators/nav1_standby_frequency_khz'


class CommandSetNav(base.CommandDataRefValue):
    rexp = re.compile(
        r'NAV_FREQ_(?P<active_mhz>\d{3})\.(?P<active_khz>\d{1,2})_(?P<standby_mhz>\d{3})\.(?P<standby_khz>\d{1,2})'
    )
    short_cmd = 'NAV_FREQ_'

    def set_value(self, value):
        match = self.rexp.search(value)
        if not match:
            utils.echo(f'Command {value!r} not matched to nav-regexp')
            return

        freq = match.groupdict()
        CommandNavActiveMhz().set_value(freq['active_mhz'])
        CommandNavActiveKhz().set_value(freq['active_khz'])
        CommandNavStandByMhz().set_value(freq['standby_mhz'])
        CommandNavStandByKhz().set_value(freq['standby_khz'])


class CommandAutoThrottle(base.Command):
    short_cmd = 'AUTO_THROTTLE_'
    # cmd = 'laminar/B738/switches/autopilot/at_arm'
    cmd = 'sim/lights/landing_lights_toggle'


class CommandAutoThrottleOn(base.Command):
    short_cmd = 'AUTO_THROTTLE_1'
    # cmd = 'laminar/B738/switches/autopilot/at_arm'
    cmd = 'sim/lights/landing_lights_on'


class CommandAutoThrottleOff(base.Command):
    short_cmd = 'AUTO_THROTTLE_0'
    # cmd = 'laminar/B738/switches/autopilot/at_arm'
    cmd = 'sim/lights/landing_lights_off'


class CommandLandingLightsOn(base.Command):
    short_cmd = 'LANDING_LIGHTS_1'
    # cmd = 'laminar/B738/switches/autopilot/at_arm'
    cmd = 'sim/lights/landing_lights_on'


class CommandLandingLightsOff(base.Command):
    short_cmd = 'LANDING_LIGHTS_0'
    # cmd = 'laminar/B738/switches/autopilot/at_arm'
    cmd = 'sim/lights/landing_lights_off'


class CommandFlightDirector(base.Command):
    short_cmd = 'FLIGHT_DIRECTOR_'
    # cmd = 'laminar/B738/switches/autopilot/fd_ca'
    cmd = 'sim/lights/taxi_lights_toggle'


class CommandFlightDirectorOn(base.Command):
    short_cmd = 'FLIGHT_DIRECTOR_1'
    # cmd = 'laminar/B738/switches/autopilot/fd_ca'
    cmd = 'laminar/B738/toggle_switch/taxi_light_brightness_on'


class CommandFlightDirectorOff(base.Command):
    short_cmd = 'FLIGHT_DIRECTOR_0'
    # cmd = 'laminar/B738/switches/autopilot/fd_ca'
    cmd = 'laminar/B738/toggle_switch/taxi_light_brightness_off'


class CommandTaxiLightsOn(base.Command):
    short_cmd = 'TAXI_LIGHTS_1'
    cmd = 'laminar/B738/toggle_switch/taxi_light_brightness_on'


class CommandTaxiLightsOff(base.Command):
    short_cmd = 'TAXI_LIGHTS_0'
    cmd = 'laminar/B738/toggle_switch/taxi_light_brightness_off'


class CommandLevelChanged(base.Command):
    short_cmd = 'LEVEL_CHANGED'
    cmd = 'laminar/B738/autopilot/lvl_chg_press'
