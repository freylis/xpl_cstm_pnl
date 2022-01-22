import xp

from .. import utils
from . import sender


class Command:
    cmd = NotImplemented
    short_cmd = NotImplemented
    sender = sender.CommandSender

    def __init__(self):
        self.command_sender = self.sender(command=self)

    def execute(self):
        utils.echo(f'1/Execute command {self}')
        xp.commandOnce(self.command)
        utils.echo(f'2/Complete command {self}')

    @property
    def command(self):
        command_obj = xp.findCommand(self.cmd.strip())
        if command_obj is None:
            raise ValueError(f'Unknown command {self.cmd}')
        return command_obj

    def send_command(self):
        command_text = self.get_command()
        utils.echo(f'Send command {command_text!r} to arduino')
        with open('D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_x.log', 'a') as f:
            f.write(command_text + '\n')

    def get_command(self):
        return f'[frey-cmd-x] {self.short_cmd}'

    def __str__(self):
        return f'<cmd {self.cmd!r}>'

    def __repr__(self):
        return self.__str__()


class CustomCommand(Command):

    def __init__(self, cmd):
        super().__init__()
        self.cmd = cmd
