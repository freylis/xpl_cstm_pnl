import xp


class Command:
    cmd = NotImplemented

    @property
    def command(self):
        command_obj = xp.findCommand(self.cmd.strip())
        if command_obj is None:
            raise ValueError(f'Unknown command {self.cmd}')
        return command_obj

    def __str__(self):
        return f'<cmd {self.cmd!r}>'

    def __repr__(self):
        return self.__str__()


class CustomCommand(Command):

    def __init__(self, cmd):
        super().__init__()
        self.cmd = cmd
