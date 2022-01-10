"""
Implementation interface between xplane and arduino-based custom panel
by freylis2
https://github.com/freylis/xpl_cstm_pnl
"""
import time
import datetime

import xp


def echo(msg):
    xp.log(msg)
    now = datetime.datetime.now()
    with open('D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log', 'a') as f:
        f.write(f'\n[{now}]: {msg}')


class PythonInterface:

    def __init__(self):
        self.Name = "xplCstmPnlFreylis"
        self.Sig = "xplCstmPnlFreylis.xppython3"
        self.Desc = "Custom arduino-based panel interface by freylis2"

    def XPluginStart(self):
        echo('Set plugin start')

        # register base commands
        CommandsRegister()

        return self.Name, self.Sig, self.Desc

    def XPluginStop(self):
        echo('Set plugin stop')

    def XPluginEnable(self):
        echo('Check plugin is enabled')
        return 1

    def XPluginDisable(self):
        echo('Check plugin is disabled')

    def XPluginReceiveMessage(self, inFromWho, inMessage, inParam):
        msg = f'Send message fh:{inFromWho} / m:{inMessage} / p:{inParam}'
        echo(msg)


class CommandsRegister:
    def __init__(self):
        self.register_gearup()
        self.register_geardown()
        self.register_throttle_up()
        self.register_throttle_down()
        self.register_flaps_up()
        self.register_flaps_down()

    def register_gearup(self):
        gearup_command = xp.findCommand('sim/flight_controls/landing_gear_up')
        xp.registerCommandHandler(gearup_command, self.on_gear_change)

    def register_geardown(self):
        geardown_command = xp.findCommand('sim/flight_controls/landing_gear_down')
        xp.registerCommandHandler(geardown_command, self.on_gear_change)

    def register_throttle_up(self):
        t_up = xp.findCommand('sim/engines/throttle_up')
        xp.registerCommandHandler(t_up, self.on_throttle_change)

    def register_throttle_down(self):
        t_down = xp.findCommand('sim/engines/throttle_down')
        xp.registerCommandHandler(t_down, self.on_throttle_change)

    def register_flaps_up(self):
        t_up = xp.findCommand('sim/flight_controls/flaps_up')
        xp.registerCommandHandler(t_up, self.on_flaps_change)
        t_p_up = xp.findCommand('sim/flight_controls/pump_flaps')
        xp.registerCommandHandler(t_p_up, self.on_flaps_change)

    def register_flaps_down(self):
        t_down = xp.findCommand('sim/flight_controls/flaps_down')
        xp.registerCommandHandler(t_down, self.on_flaps_change)

    @staticmethod
    def on_gear_change(commandRef, phase, refCon):
        echo(f"Gear changed: p:{phase} / rc:{refCon}")
        return 1

    @staticmethod
    def on_throttle_change(commandRef, phase, refCon):
        echo(f"Throttle changed: p:{phase} / rc:{refCon}")

        throttle_ref = xp.findDataRef('sim/cockpit2/engine/actuators/throttle_ratio_all')
        vals = (
            xp.getDatai(throttle_ref),
            xp.getDataf(throttle_ref),
            xp.getDatad(throttle_ref),
        )
        echo(f'Current throttle: {vals}')
        return 1

    @staticmethod
    def on_flaps_change(commandRef, phase, refCon):
        echo(f"Flaps changed: p:{phase} / rc:{refCon}")

        # get actual flaps level
        flap_ref = xp.findDataRef('sim/flightmodel/controls/flaprqst')
        vals = (
            xp.getDatai(flap_ref),
            xp.getDataf(flap_ref),
            xp.getDatad(flap_ref),
        )
        echo(f'Current flaps: {vals}')
        return 1
