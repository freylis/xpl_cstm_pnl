import xp

from .. import utils
from .. import commands as command_utils


class Callback:

    def __init__(self, *commands):
        self.commands = commands

    def set(self, silent=False):
        success_commands = []
        for cmd in self.commands:
            try:
                xp.registerCommandHandler(cmd.command, self.callback, before=0)
            except ValueError:
                utils.echo(f'Unknown command {cmd.cmd}', error=True)
                if not silent:
                    raise
                continue
            success_commands.append(cmd)

    @classmethod
    def callback(cls, commandRef, phase, refCon):
        command_utils.CommandFullState().send_command()
        return 1
