import xp

from .. import utils


class Callback:

    def __init__(self, *commands):
        self.commands = commands

    def set(self, silent=False):
        success_commands = []
        for cmd in self.commands:
            try:
                xp.registerCommandHandler(cmd.command, self.callback)
                xp.registerCommandHandler(cmd.command, self.callback, before=0)
            except ValueError:
                utils.echo(f'Unknown command {cmd.cmd}', error=True)
                if not silent:
                    raise
                continue
            success_commands.append(cmd)
        utils.echo(f'Subscribed {success_commands!r} for {self}')

    def callback(self, commandRef, phase, refCon):
        return 1
