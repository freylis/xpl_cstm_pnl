"""
Implementation interface between xplane and arduino-based custom panel
by freylis2
https://github.com/freylis/xpl_cstm_pnl
"""
import xp

import frey_utils


log = frey_utils.echo


class PythonInterface:

    def __init__(self):
        self.Name = "xplCstmPnlFreylis"
        self.Sig = "xplCstmPnlFreylis.xppython3"
        self.Desc = "Custom arduino-based panel interface by freylis2"

    def XPluginStart(self):
        log('Set plugin start')

        # register callbacks for plane changes
        self._register_callbacks()

        return self.Name, self.Sig, self.Desc

    def XPluginStop(self):
        log('Set plugin stop')

    def XPluginEnable(self):
        log('Check plugin is enabled')
        return 1

    def XPluginDisable(self):
        log('Check plugin is disabled')

    def XPluginReceiveMessage(self, inFromWho, inMessage, inParam):
        msg = f'Send message fh:{inFromWho} / m:{inMessage} / p:{inParam}'
        log(msg)

    # @frey_utils.error_log
    def _register_callbacks(self):
        engine_callback = frey_utils.CallbackThrottleChange(
            frey_utils.CommandThrottleUp(),
            frey_utils.CommandThrottleDown(),
        )
        engine_callback.set_callbacks()

        flaps_callback = frey_utils.CallbackFlapsUpDown(
            frey_utils.CommandFlapsUp(),
            frey_utils.CommandFlapsDown(),
        )
        flaps_callback.set_callbacks()

        gear_callback = frey_utils.CallbackGearUpDown(
            frey_utils.CommandGearUp(),
            frey_utils.CommandGearDown(),
            frey_utils.CommandGearToggle(),
        )
        gear_callback.set_callbacks()

        # register scheduled callback
        # schedule_loop_id = xp.createFlightLoop(frey_utils.scheduled_callback)
        # xp.scheduleFlightLoop(schedule_loop_id, interval=0.1)
