import xp

from .. import utils


class Callback:

    def __init__(self, *commands):
        self.commands = commands

    def set(self):
        for cmd in self.commands:
            xp.registerCommandHandler(cmd.command, self.callback)

    @classmethod
    def callback(cls, commandRef, phase, refCon):
        utils.echo(f'Callback for {commandRef} / p:{phase!r}')
        return 1
