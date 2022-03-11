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
    to_register_commands = [
        frey_utils.CommandFullState,
    ]

    def __init__(self):
        self.Name = "xplCstmPnlFreylis"
        self.Sig = "xplCstmPnlFreylis.xppython3"
        self.Desc = "Custom arduino-based panel interface by freylis2"

    def XPluginStart(self):
        log('Set plugin start')

        # load from config
        self._config_handle()
        # register callbacks for plane changes
        self._register_callbacks()
        xp.speakString("[frey] Starting panel plugin")
        return self.Name, self.Sig, self.Desc

    def XPluginStop(self):
        log('Set plugin stop')
        xp.speakString("[frey] Stop panel plugin")

    def XPluginEnable(self):
        log('Check plugin is enabled')
        return 1

    def XPluginDisable(self):
        log('Check plugin is disabled')

    def XPluginReceiveMessage(self, inFromWho, inMessage, inParam):
        pass

    @frey_utils.error_log
    def _config_handle(self):
        # register my custom commands
        for cmd in [
            frey_utils.CommandFullState,
        ]:
            frey_utils.echo(f'Register custom command {cmd}')
            cmd.register()

        # subscribe callbacks to commands
        good, bad = 0, 0
        for callback_path, commands_set in frey_utils.CALLBACK_STRUCTURE.items():
            try:
                callback_cls = frey_utils.import_smthng(callback_path)
            except ImportError:
                frey_utils.echo(f'Cant import {callback_path!r}. Not found', error=True)
                bad += 0
                continue

            commands = []
            for cmd in commands_set:
                if isinstance(cmd, str):
                    command = frey_utils.CustomCommand(cmd)
                elif isinstance(cmd, frey_utils.Command):
                    command = cmd
                else:
                    frey_utils.echo(f"Cant get command {cmd}")
                    continue
                commands.append(command)
                good += 1
            callback = callback_cls(*commands)
            callback.set(silent=True)
        xp.speakString("[frey] Subscribed {good} / {bad} events")

    def _register_callbacks(self):
        # register scheduled callback
        schedule_loop_id = xp.createFlightLoop(frey_utils.scheduled_callback)
        xp.scheduleFlightLoop(schedule_loop_id, interval=2)
        xp.speakString("[frey] Run loop event")
