import re

import xp

from .. import utils


class Command:
    cmd = NotImplemented
    short_cmd = NotImplemented

    def execute(self):
        utils.echo(f'Send command {self.command}')
        xp.commandOnce(self.command)

    @property
    def command(self):
        command_obj = xp.findCommand(self.cmd.strip())
        if command_obj is None:
            raise ValueError(f'Unknown command {self.cmd}')
        return command_obj

    def send_command(self):
        command_text = self.get_command()
        with open('D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey_cmd_x.log', 'a') as f:
            f.write(command_text + '\n')

    def get_command(self):
        return f'[frey-cmd-x] {self.short_cmd}'

    def __str__(self):
        return f'<cmd {self.cmd!r} / {self.__class__.__name__}>'

    def __repr__(self):
        return self.__str__()


class CustomCommand(Command):

    def __init__(self, cmd):
        super().__init__()
        self.cmd = cmd


class CommandDataRefValue(Command):
    i_rexp = re.compile(r'[^\d]*(\d+)$')

    def set_value(self, value):
        dref = xp.findDataRef(self.cmd)
        if not dref:
            utils.echo(f'Unknown dataref={self.cmd} for execute command {self}', error=True)
            return

        utils.echo(f'Set dref {self.cmd!r}={value}')
        if isinstance(value, int):
            xp.setDatai(dref, value)
        elif isinstance(value, float):
            xp.setDataf(dref, value)
        elif isinstance(value, str):
            xp.setDatas(value, value)
        else:
            utils.echo(f'Unknown dataref setter for {value!r} (type {type(value)})', error=True)
            return

    def set_last_int(self, value):
        match = self.i_rexp.search(value)
        if not match:
            utils.echo(f'Cant get ...int from {value!r}')
            return
        self.set_value(match.groups()[0])


class CommandDataRefIntegerValue(CommandDataRefValue):

    def set_value(self, value):
        dref = xp.findDataRef(self.cmd)
        if not dref:
            utils.echo(f'Unknown dataref={self.cmd} for execute command {self}', error=True)
            return
        match = self.i_rexp.search(value)
        if not match:
            utils.echo(f'Cant get ...int from {value!r}')
            return

        new_value = match.groups()[0]
        utils.echo(f'Set dref {self.cmd!r}={value}')
        self._set_value(dref, int(new_value))

    def _set_value(self, dataref, value):
        xp.setDatai(dataref, value)


class CommandDataRefFloatValue(CommandDataRefIntegerValue):

    def _set_value(self, dataref, value):
        utils.echo(f'Set {self.cmd}={float(value)}')
        xp.setDataf(dataref, float(value))
