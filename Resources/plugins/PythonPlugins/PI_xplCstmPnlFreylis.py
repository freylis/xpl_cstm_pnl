"""
Implementation interface between xplane and arduino-based custom panel
by freylis2
https://github.com/freylis/xpl_cstm_pnl
"""
import xp

import frey_utils


log = frey_utils.echo


class PythonInterface:
    cfg_filename = 'cstm_pnl_frey_config.yml'

    def __init__(self):
        self.Name = "xplCstmPnlFreylis"
        self.Sig = "xplCstmPnlFreylis.xppython3"
        self.Desc = "Custom arduino-based panel interface by freylis2"

    def XPluginStart(self):
        log('Set plugin start')

        # load from config
        self._config_handle()
        # register callbacks for plane changes
        # self._register_callbacks()

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

    @frey_utils.error_log
    def _config_handle(self):
        # subscribe callbacks to commands
        for callback_path, commands_set in frey_utils.CALLBACK_STRUCTURE.items():
            try:
                callback_cls = frey_utils.import_smthng(callback_path)
            except ImportError:
                frey_utils.echo(f'Cant import {callback_path!r}. Not found')
                continue

            commands = []
            for cmd in commands_set:
                command = frey_utils.CustomCommand(cmd)
                commands.append(command)
            callback = callback_cls(*commands)
            callback.set()
            frey_utils.echo(f'Subscribed {commands!r} for {callback_path!r}')

    def _register_callbacks(self):
        pass
        # register scheduled callback
        # schedule_loop_id = xp.createFlightLoop(frey_utils.scheduled_callback)
        # xp.scheduleFlightLoop(schedule_loop_id, interval=0.1)
