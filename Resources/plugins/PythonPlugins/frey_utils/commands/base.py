import xp


class Command:
    cmd = NotImplemented

    @property
    def command(self):
        command_obj = xp.findCommand(self.cmd)
        if command_obj is None:
            raise ValueError(f'Unknown command {self.cmd}')
        return command_obj

    def __str__(self):
        return f'<cmd {self.cmd!r}>'
